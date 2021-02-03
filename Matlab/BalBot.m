classdef BalBot < handle
    %BALBOT Bluetooth interface for self-balancing robot
    %   
    %   Author: Dan Oates (WPI Class of 2020)
    
    properties (SetAccess = protected)
        lin_vel_lim;    % Linear velocity limiter [controls.ClampLimiter]
        lin_acc_lim;    % Linear acceleration limiter [controls.SlewLimiter]
        yaw_vel_lim;    % Yaw velocity limiter [controls.ClampLimiter]
    end
    
    properties (Access = protected)
        serial_;    % Embedded serial interface [serial_com.SerialStruct]
    end
    
    methods (Access = public)
        function obj = BalBot(bot_name, lin_vel_max, lin_acc_max, yaw_vel_max)
            %obj = BALBOT(bot_name, lin_vel_max, lin_acc_max, yaw_vel_max)
            %   Construct robot interface
            %   
            %   Inputs:
            %   - bot_name = Bluetooth name [ex. 'ES3011_BOT01']
            %   - lin_vel_max = Max linear velocity command [m/s]
            %   - lin_acc_max = Max linear acceleration command [m/s^2]
            %   - yaw_vel_max = Max yaw velocity command [rad/s]
            serial_ = serial_com.make_bluetooth(bot_name);
            obj.serial_ = serial_com.SerialStruct(serial_);
            obj.lin_vel_lim = controls.ClampLimiter(lin_vel_max);
            obj.lin_acc_lim = controls.SlewLimiter(lin_acc_max);
            obj.yaw_vel_lim = controls.ClampLimiter(yaw_vel_max);
        end
        
        function state = send_cmds(obj, lin_vel_cmd, yaw_vel_cmd)
            %state = SEND_CMDS(obj, lin_vel_cmd, yaw_vel_cmd)
            %   Send commands and get robot state
            %   
            %   Inputs:
            %   - lin_vel_cmd = Linear velocity [m/s]
            %   - yaw_vel_cmd = Yaw velocity [rad/s]
            %   
            %   Outputs:
            %   - state.lin_vel_cmd = Limited linear velocity [m/s]
            %   - state.yaw_vel_cmd = Limited yaw velocity [rad/s]
            %   - state.lin_vel = Robot linear velocity [m/s]
            %   - state.yaw_vel = Robot yaw velocity [rad/s]
            %   - state.volts_L = Left motor voltage [V]
            %   - state.volts_R = Right motor voltage [V]
            
            % Filter input commands
            lin_vel_cmd = obj.lin_vel_lim.update(lin_vel_cmd);
            lin_vel_cmd = obj.lin_acc_lim.update(lin_vel_cmd);
            yaw_vel_cmd = obj.yaw_vel_lim.update(yaw_vel_cmd);
            
            % Send filtered commands
            obj.serial_.write(lin_vel_cmd, 'single');
            obj.serial_.write(yaw_vel_cmd, 'single');
            
            % Get state from robot
            state = struct();
            state.lin_vel_cmd = lin_vel_cmd;
            state.yaw_vel_cmd = yaw_vel_cmd;
            state.lin_vel = obj.serial_.read('single');
            state.yaw_vel = obj.serial_.read('single');
            state.volts_L = obj.serial_.read('single');
            state.volts_R = obj.serial_.read('single');
        end
        
        function delete(obj)
            %DELETE(obj) Disconnects from Bluetooth
            fclose(obj.serial_.get_serial());
        end
    end
end