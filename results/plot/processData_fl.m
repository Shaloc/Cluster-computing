%对测试数据进行分析，分为定点和浮点，包括：
%计算量不变，加速比和节点数关系
%节点数不变，加速比和计算量关系
%每秒计算速度op/s
%节点数不变，每秒计算速度和计算量关系
%计算量不变，每秒计算速度和节点数关系


clc;
clear
close all
%取得浮点运算的数据
cluster_data_fl

%计算加速比
[M,N] = size(muti);
acc = zeros(M,N);
for i = 1:M
    for j = 1:N
        acc(i,j) = single(i) / muti(i,j);
    end
end
%三维
node = [2:2:16];
surf(node,iter,acc),zlabel('加速比'),xlabel('节点数'),ylabel('迭代次数'),shading interp
saveas(gcf,['./3D_fl','.png']);
figure
%不动节点数，改变计算量
for i = 1:8
    plot(iter,acc(:,i)),xlabel('迭代次数'),ylabel('加速比'),title('不同节点数下加速比和迭代次数的关系'),hold on;
end
legend('node:2','node:4','node:6','node:8','node:10','node:12','node:14','node:16');
saveas(gcf,['./acc-iter_fl','.png']);
%不同节点数下加速比和迭代次数的关系，分开画
rmdir('./acc-iter_fl','s')
mkdir('./acc-iter_fl/')
for i = 1:8
    figure
    plot(iter,acc(:,i)),axis([-inf,inf,0,20]),xlabel('迭代次数'),ylabel('加速比'),title([num2str(2*i),'个节点'])
    saveas(gcf,['./acc-iter_fl/','节点数',num2str(node(i)),'.png']);
end
%不同迭代次数下加速比和节点数量的关系，分开画
rmdir('./acc-node_fl','s')
mkdir('./acc-node_fl/')
for i = 1:52
    figure
    plot(node,acc(i,:)),axis([-inf,inf,0,20]),xlabel('节点数'),ylabel('加速比'),title(['迭代',num2str(iter(i)),'次'])
    saveas(gcf,['./acc-node_fl/','迭代次数',num2str(iter(i)),'.png']);
end





