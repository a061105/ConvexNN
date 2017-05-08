% This make.m is for MATLAB and OCTAVE under Windows, Mac, and Unix

try
	Type = ver;
	mex CFLAGS="\$CFLAGS -std=c99" -largeArrayDims InputLayerSolve.cpp tron.cpp blas/daxpy.c blas/ddot.c blas/dnrm2.c blas/dscal.c
catch
	fprintf('If make.m fails, please check README about detailed instructions.\n');
	msg = getReport(exception);
	msg
end
