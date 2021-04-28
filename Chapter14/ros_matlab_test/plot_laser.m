function plot_laser( )
    global laser_msg;
    %ROS_MASTER_URI
    rosinit
    % wait a bit the roscore initialization
    pause(2)     
    
    laser_sub = rossubscriber( '/scan', @get_laser );
    r = rosrate(2); % 2 Hz loop rate 
    for i=1:50
        plot(laser_msg   ); %Plot laser_msg 
        waitfor(r);
    end
    %Shutdown ROS connection
    rosshutdown
    close all
end
