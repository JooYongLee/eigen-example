#include <Eigen>
#include <vector>
#include <fstream>
#include <iostream>
using namespace Eigen;
template <typename T>
inline T clip(T x, T a, T b) { return std::min(std::max(x, a), b); }

template<typename Derived>
void digitize(MatrixBase<Derived> &mat, 
	VectorXi &hist, 
	int nBins, 
	VectorXi *digitInds=nullptr)
{
	auto dmin = mat.minCoeff();
	auto dmax = mat.maxCoeff();
	//int hist_size = 1000;
	auto dscale = nBins / (dmax - dmin);
	auto rescale = (mat.array() - dmin) * dscale;
	//cout << rescale.minCoeff() << "," << rescale.maxCoeff() << endl;
	auto y = (mat.array() - dmin) * dscale;
	if (digitInds)
	{
		*digitInds = y.cast<int>();
	}
	
	//MatrixXi hist = MatrixXi::Zero(1, )
	hist = VectorXi::Zero(nBins);
	int nMaxIndex = nBins - 1;
	for (int i = 0; i < y.size(); i++)
	{
		int floor_v = (int)y(i);
		hist(clip<int>(floor_v, 0, nMaxIndex))++;
	}
}
void test_digitize(VectorXd &mat, VectorXi &inds)
{
	int nBins = inds.size();
	//MatrixXd mat = MatrixXd::Random(nBins, 1);
	VectorXi hist;
	VectorXi digit_inds;
	digitize(mat, hist, inds.size(), &digit_inds);

	std::cout << "digit inds" << digit_inds.minCoeff()
		<< "," << digit_inds.maxCoeff() << std::endl;
	//cout << hist - inds << endl;

	std::cout << "diff" << (hist - inds).cwiseAbs().sum();
	//std::cout << hist << std::endl;
}

void get_test_histogram_data(std::vector<double> &hist, std::vector<int> &inds)
{
	hist.reserve(50000);
	inds.reserve(1000);
	std::fstream file, file2;
	file.open("test.txt", std::ios::in);
	std::string line;
	if (file.is_open())
	{
		while (getline(file, line))
		{
			double d = atof(line.c_str());
			hist.push_back(d);
		}
		file.close();
	}
	file2.open("hist.txt", std::ios::in);
	if (file2.is_open())
	{
		//cout << "open integer" << endl;
		while (getline(file2, line))
		{

			int d = atoi(line.c_str());
			//cout << d << endl;
			inds.push_back(d);
		}
		file2.close();
	}	
	std::cout << hist.size() << "," << inds.size() << std::endl;
}