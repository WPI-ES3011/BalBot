function balbot_teleop(...
    bot_name, ...
    ctrl_type, ...
    lin_vel_max, ...
    lin_acc_max, ...
    yaw_vel_max)
%BALBOT_TELEOP(bot_name, ctrl_type, lin_vel_max, lin_acc_max, yaw_vel_max)
%   Run bluetooth teleoperation of self-balancing robot
%   
%   Inputs:
%   - bot_name = Bluetooth device name [ex. 'ES3011_BOT01']
%   - ctrl_type = Controller type ['Xbox360', 'Nint64']
%   - lin_vel_max = Max linear velocity command [m/s]
%   - lin_acc_max = Max linear acceleration command [m/s^2]
%   - yaw_vel_max = Max yaw velocity command [rad/s]
clc, instrreset;

% Default arguments
if nargin < 1, bot_name = 'BalBot'; end
if nargin < 2, ctrl_type = 'Xbox360'; end
if nargin < 3, lin_vel_max = 0.8; end
if nargin < 4, lin_acc_max = 0.8; end
if nargin < 5, yaw_vel_max = 1.6; end

% Create game controller
switch ctrl_type
    case 'Xbox360', ctrl = game_ctrl.Xbox360();
    case 'Nint64', ctrl = game_ctrl.Nint64();
    otherwise, error('Invalid controller type')
end
ctrl.auto_dzs();

% Connect to robot
fprintf('Balancing Robot Controller\n\n')
fprintf(['Connecting to robot ''' bot_name ''' ...\n'])
balbot = BalBot(bot_name, lin_vel_max, lin_acc_max, yaw_vel_max);

% Log vectors
log_n = 10000;
lin_vel_cmd = zeros(log_n, 1);
yaw_vel_cmd = zeros(log_n, 1);
lin_vel = zeros(log_n, 1);
yaw_vel = zeros(log_n, 1);
volts_L = zeros(log_n, 1);
volts_R = zeros(log_n, 1);
t = zeros(log_n, 1);

% Communication loop
timer = timing.Timer();
com_error = 0;
log_i = 1;
while 1
    
    % Get commands from game controller
    switch ctrl_type
        case 'Xbox360'
            lin_vel_cmd(log_i) = -lin_vel_max * ctrl.axis('Trig');
            yaw_vel_cmd(log_i) = -yaw_vel_max * ctrl.axis('LX');
        case 'Nint64'
            lin_vel_cmd(log_i) = lin_vel_max * ctrl.axis('JY');
            yaw_vel_cmd(log_i) = -yaw_vel_max * ctrl.axis('JX');
    end
    
    % Robot communication
    try
        % Send commands to robot and get state
        state = balbot.send_cmds(lin_vel_cmd(log_i), yaw_vel_cmd(log_i));
        lin_vel_cmd(log_i) = state.lin_vel_cmd;
        yaw_vel_cmd(log_i) = state.yaw_vel_cmd;
        
        % Display state
        clc
        fprintf('Balancing Robot Controller\n')
        fprintf('Press B to exit.\n\n')
        fprintf('Lin vel cmd: %+.2f [m/s]\n', lin_vel_cmd(log_i))
        fprintf('Lin vel: %+.2f [m/s]\n', state.lin_vel)
        fprintf('Yaw vel cmd: %+.2f [rad/s]\n', yaw_vel_cmd(log_i))
        fprintf('Yaw vel: %+.2f [rad/s]\n', state.yaw_vel)
        fprintf('Voltage L: %+.1f [V]\n', state.volts_L)
        fprintf('Voltage R: %+.1f [V]\n', state.volts_R)

        % Log state
        lin_vel(log_i) = state.lin_vel;
        yaw_vel(log_i) = state.yaw_vel;
        volts_L(log_i) = state.volts_L;
        volts_R(log_i) = state.volts_R;
        t(log_i) = timer.toc();
    catch
        % Communication error
        log_i = log_i - 1;
        com_error = 1;
    end
    
    % Exit conditions
    exit_btn = ctrl.btn('B');
    if exit_btn || com_error
        lin_vel_cmd = lin_vel_cmd(1:log_i);
        lin_vel = lin_vel(1:log_i);
        yaw_vel_cmd = yaw_vel_cmd(1:log_i);
        yaw_vel = yaw_vel(1:log_i);
        volts_L = volts_L(1:log_i);
        volts_R = volts_R(1:log_i);
        t = t(1:log_i);
        if exit_btn
            fprintf('\nProgram terminated by user.\n')
        elseif com_error
            fprintf('\nBluetooth communication error.\n')
        end
        break
    end
    if log_i == log_n
        fprintf('\nProgram terminated automatically.\n')
        break
    end
    
    % Increment log index
    log_i = log_i + 1;
end

% Stop and disconnect from robot
balbot.send_cmds(0, 0);

% Plot results
figure(1)
clf

% Plot Linear Velocity
subplot(2, 2, 1)
hold on, grid on
title('Linear Velocity')
xlabel('Time [s]')
ylabel('Velocity [m/s]')
plot(t, lin_vel_cmd, 'k--')
plot(t, lin_vel, 'b-')
legend('Setpt', 'Value')

% Plot Yaw Velocity
subplot(2, 2, 2)
hold on, grid on
title('Yaw Velocity')
xlabel('Time [s]')
ylabel('Velocity [rad/s]')
plot(t, yaw_vel_cmd, 'k--')
plot(t, yaw_vel, 'b-')
legend('Setpt', 'Value')

% Plot Voltage-L
subplot(2, 2, 3)
hold on, grid on
title('Voltage-L')
xlabel('Time [s]')
ylabel('Voltage [V]')
plot(t, volts_L, 'r-')

% Plot Voltage-R
subplot(2, 2, 4)
hold on, grid on
title('Voltage-R')
xlabel('Time [s]')
ylabel('Voltage [V]')
plot(t, volts_R, 'r-')

end