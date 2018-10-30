%�Բ������ݽ��з�������Ϊ����͸��㣬������
%���������䣬���ٱȺͽڵ�����ϵ
%�ڵ������䣬���ٱȺͼ�������ϵ
%ÿ������ٶ�op/s
%�ڵ������䣬ÿ������ٶȺͼ�������ϵ
%���������䣬ÿ������ٶȺͽڵ�����ϵ


clc;
clear
close all
%ȡ�ø������������
cluster_data_fl

%������ٱ�
[M,N] = size(muti);
acc = zeros(M,N);
for i = 1:M
    for j = 1:N
        acc(i,j) = single(i) / muti(i,j);
    end
end
%��ά
node = [2:2:16];
surf(node,iter,acc),zlabel('���ٱ�'),xlabel('�ڵ���'),ylabel('��������'),shading interp
saveas(gcf,['./3D_fl','.png']);
figure
%�����ڵ������ı������
for i = 1:8
    plot(iter,acc(:,i)),xlabel('��������'),ylabel('���ٱ�'),title('��ͬ�ڵ����¼��ٱȺ͵��������Ĺ�ϵ'),hold on;
end
legend('node:2','node:4','node:6','node:8','node:10','node:12','node:14','node:16');
saveas(gcf,['./acc-iter_fl','.png']);
%��ͬ�ڵ����¼��ٱȺ͵��������Ĺ�ϵ���ֿ���
rmdir('./acc-iter_fl','s')
mkdir('./acc-iter_fl/')
for i = 1:8
    figure
    plot(iter,acc(:,i)),axis([-inf,inf,0,20]),xlabel('��������'),ylabel('���ٱ�'),title([num2str(2*i),'���ڵ�'])
    saveas(gcf,['./acc-iter_fl/','�ڵ���',num2str(node(i)),'.png']);
end
%��ͬ���������¼��ٱȺͽڵ������Ĺ�ϵ���ֿ���
rmdir('./acc-node_fl','s')
mkdir('./acc-node_fl/')
for i = 1:52
    figure
    plot(node,acc(i,:)),axis([-inf,inf,0,20]),xlabel('�ڵ���'),ylabel('���ٱ�'),title(['����',num2str(iter(i)),'��'])
    saveas(gcf,['./acc-node_fl/','��������',num2str(iter(i)),'.png']);
end





