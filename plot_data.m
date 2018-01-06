function[] = plot_data()

x = dlmread('input2');
x = x(2:end,:);

centroids = dlmread('centroids');
init_c = dlmread('init_centroids');

a = zeros(3,2);

hold on
plot(init_c(:,1),init_c(:,2),'o','color','m','MarkerFaceColor', 'm');
plot(centroids(:,1),centroids(:,2),'o','color','b','MarkerFaceColor', 'b');
plot(init_c(1,1),centroids(1,1),init_c(1,2),centroids(1,2),'-');
plot([init_c(1,1), centroids(1,1)],[init_c(1,2), centroids(1,2)],'-','color','black');
plot([init_c(2,1), centroids(2,1)],[init_c(2,2), centroids(2,2)],'-','color','black');
plot([init_c(3,1), centroids(3,1)],[init_c(3,2), centroids(3,2)],'-','color','black');
plot([init_c(4,1), centroids(4,1)],[init_c(4,2), centroids(4,2)],'-','color','black');
plot([init_c(5,1), centroids(5,1)],[init_c(5,2), centroids(5,2)],'-','color','black');
plot([init_c(6,1), centroids(6,1)],[init_c(6,2), centroids(6,2)],'-','color','black');




plot(x(:,1),x(:,2),'*')
legend('Initial Centroids','Final Centroids');
xlabel('x');
ylabel('y');
hold off

end