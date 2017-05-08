function loss = test_train(lambda,V)

[N,D] = size(V);

W = zeros(1,D);
for j = 1:D
				model=train( full(V(:,j)),sparse(ones(N,1)),lambda); 
				W(:,j)=model.w'; 
end

Z=ones(N,1)*W;
loss = sum(sum(log(1+exp(Z))-V.*Z));
