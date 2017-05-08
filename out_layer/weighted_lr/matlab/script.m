[y,V] = libsvmread('../../binary_data/binary_data');

lambda = 1e10
loss1=test_train(lambda, V)

lambda = 1e3
loss2=test_train(lambda, V)
