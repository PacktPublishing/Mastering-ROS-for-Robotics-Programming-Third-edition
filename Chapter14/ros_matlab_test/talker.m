%ROS_MASTER_URI
rosinit
% wait a bit the roscore initialization
pause(2) 
%Subscribe to the topic: /talker
talker_sub = rossubscriber( '/talker' );
%Advertise a String on topic /chatter
[chatter_pub, chatter_msg] = rospublisher('/chatter','std_msgs/String');
r = rosrate(2); % 2 Hz loop rate
pause(2) % wait a bit the roscore initialization
for i = 1:20
    %Get data from the input topic
    data = talker_sub.LatestMessage;
    chatter_msg.Data = data.Data;
    %Publish data on the output topic
    send(chatter_pub, chatter_msg);
    %Wait for the control loop rate
    waitfor(r);
end
%Shutdown ROS connection
rosshutdown