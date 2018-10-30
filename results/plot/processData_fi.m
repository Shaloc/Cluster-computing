%�Բ������ݽ��з�������Ϊ����͸��㣬������
%���������䣬���ٱȺͽڵ�����ϵ
%�ڵ������䣬���ٱȺͼ�������ϵ
%ÿ������ٶ�op/s
%�ڵ������䣬ÿ������ٶȺͼ�������ϵ
%���������䣬ÿ������ٶȺͽڵ�����ϵ


clc;
clear
close all
%ȡ�ö������������
cluster_data_fi
%������ٱ�
[M,N] = size(muti);
acc = zeros(M,N);
for i = 1:M
    for j = 1:N
        acc(i,j) = single(i,j) / muti(i,j);
    end
end
%�����ڵ��������ı�����С
for i = 1:8
    plot(iter(i,:),acc(:,i)),xlabel('�����С'),ylabel('���ٱ�'),title('��ͬ�ڵ����¼��ٱȺ;����С�Ĺ�ϵ'),hold on;
end
legend('node:2','node:4','node:6','node:8','node:10','node:12','node:14','node:16');
saveas(gcf,['./acc-size_fi','.png']);
%�����ڵ��������ı�����С���ֿ���ͼ
mkdir('./acc-size_fi/')
for i = 1:8
    figure
    plot(iter(i,:),acc(:,i)),axis([-inf,inf,0,14]),xlabel('�����С'),ylabel('���ٱ�'),title(['node:',num2str(2*i)])
    saveas(gcf,['./acc-size_fi/',num2str(node(i)),'.png']);
end
%���������С���ı�ڵ��������ֿ���ͼ
mkdir('./acc-node_fi/')
for i = 1:12
    figure
    plot(node,acc(i,:)),axis([-inf,inf,0,14]),xlabel('�ڵ�����'),ylabel('���ٱ�')
    saveas(gcf,['./acc-node_fi/',num2str(i),'.png']);
end







