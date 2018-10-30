%对测试数据进行分析，分为定点和浮点，包括：
%计算量不变，加速比和节点数关系
%节点数不变，加速比和计算量关系
%每秒计算速度op/s
%节点数不变，每秒计算速度和计算量关系
%计算量不变，每秒计算速度和节点数关系


clc;
clear
close all
%取得定点运算的数据
cluster_data_fi
%计算加速比
[M,N] = size(muti);
acc = zeros(M,N);
for i = 1:M
    for j = 1:N
        acc(i,j) = single(i,j) / muti(i,j);
    end
end
%不动节点数量，改变矩阵大小
for i = 1:8
    plot(iter(i,:),acc(:,i)),xlabel('矩阵大小'),ylabel('加速比'),title('不同节点数下加速比和矩阵大小的关系'),hold on;
end
legend('node:2','node:4','node:6','node:8','node:10','node:12','node:14','node:16');
saveas(gcf,['./acc-size_fi','.png']);
%不动节点数量，改变矩阵大小，分开画图
mkdir('./acc-size_fi/')
for i = 1:8
    figure
    plot(iter(i,:),acc(:,i)),axis([-inf,inf,0,14]),xlabel('矩阵大小'),ylabel('加速比'),title(['node:',num2str(2*i)])
    saveas(gcf,['./acc-size_fi/',num2str(node(i)),'.png']);
end
%不动矩阵大小，改变节点数量，分开画图
mkdir('./acc-node_fi/')
for i = 1:12
    figure
    plot(node,acc(i,:)),axis([-inf,inf,0,14]),xlabel('节点数量'),ylabel('加速比')
    saveas(gcf,['./acc-node_fi/',num2str(i),'.png']);
end







