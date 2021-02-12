function balbot_serial(...
    team_name, ...
    bot_name, ...
    lin_vel_max, ...
    lin_acc_max, ...
    yaw_vel_max)
%BALBOT_SERIAL(team_name, bot_name, lin_vel_max, lin_acc_max, yaw_vel_max)
%   Run serial control of self-balancing robot
%   
%   Inputs:
%   - team_name = Team ID [ex. 'Team 1']
%   - bot_name = Robot ID name [ex. 'ES3011_BOT01']
%   - lin_vel_max = Max linear velocity command [m/s]
%   - lin_acc_max = Max linear acceleration command [m/s^2]
%   - yaw_vel_max = Max yaw velocity command [rad/s]
clc, instrreset;

% Default arguments
if nargin < 1, team_name = 'Team No-Name'; end
if nargin < 2, bot_name = 'BalBot'; end
if nargin < 3, lin_vel_max = 0.8; end
if nargin < 4, lin_acc_max = 0.8; end
if nargin < 5, yaw_vel_max = 1.6; end


% Connect to robot
fprintf(['Balancing Robot Controller\n\n'])
fprintf(['Connecting to robot ''' bot_name ''' ...\n'])
balbot = BalBot(bot_name, lin_vel_max, lin_acc_max, yaw_vel_max);

% Log vectors
log_n = 10000;
lin_vel_cmd = zeros(log_n, 1);
yaw_vel_cmd = zeros(log_n, 1);
pitch = zeros(log_n, 1);
lin_vel = zeros(log_n, 1);
yaw_vel = zeros(log_n, 1);
volts_L = zeros(log_n, 1);
volts_R = zeros(log_n, 1);
t = zeros(log_n, 1);

% Communication loop
timer = timing.Timer();
com_error = 0;
log_i = 1;
exit_btn = 0;
while 1
    
    % Robot communication
    try
        % Send commands to robot and get state
        state = balbot.send_cmds(lin_vel_cmd(log_i), yaw_vel_cmd(log_i));
        lin_vel_cmd(log_i) = state.lin_vel_cmd;
        yaw_vel_cmd(log_i) = state.yaw_vel_cmd;
        
        % Display state
        clc
        fprintf(['Balancing Robot Controller for ''' team_name ''' \n\n'])
        fprintf('Pitch: %+.2f [rad]\n', state.pitch)
%         fprintf('Lin vel: %+.2f [m/s]\n', state.lin_vel)
%         fprintf('Yaw vel: %+.2f [rad/s]\n', state.yaw_vel)
%         fprintf('Voltage L: %+.1f [V]\n', state.volts_L)
%         fprintf('Voltage R: %+.1f [V]\n', state.volts_R)

        % Log state
        pitch(log_i) = state.pitch;
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
    
    % Increment log index
    log_i = log_i + 1;
end

end