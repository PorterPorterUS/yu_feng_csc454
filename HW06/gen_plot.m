% v8 characterization. %
clear all
close all

data = importdata ('data.txt');

for i = 1:11
    tmp = data((i-1)*6+1:6*i,2:3);
    accum = data((i-1)*6+1:6*i,1).*data((i-1)*6+1:6*i,3);
    figure(1)
    plot(log10(tmp(:,1)), log(accum));
    hold on;
end

xlabel('log10 (nodes)')
ylabel('log_e (time)')
legend('1 node','2 node', '3 node', '4 node', '6 node', '8 node', ...
    '12 node', '16 node', '24 node', '32 node','48 node', 'Location','northwest');
hold off

