% PID controller parameters
Kp = 1;
Ki = 1;
Kd = 1;

% define the transfer function of the plant
num = [0.5];
den = [1 2 1];
plant_tf = tf(num,den);

% PID controller transfer function
pid_tf = pid(Kp,Ki,Kd);

% loop until Ctrl-C is pressed
while true
    % read sensor data and calculate error
    sensor_data = read_sensor();
    error = target_value - sensor_data;

    % calculate PID control output
    output = pid_tf*error;

    % write output to file
    fid = fopen('output.txt', 'w');
    fprintf(fid, '%f\n', output);
    fclose(fid);

    % wait for some time
    pause(0.1);
end
