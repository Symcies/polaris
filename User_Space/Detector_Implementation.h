#include "Sensor_Prototype.h"

namespace Detector_Components
{
	namespace Types
	{
		/// This is an implementation of outliers detection for 1-dimensional data set based on Statistical Test (http://www.siam.org/meetings/sdm10/tutorial3.pdf). The assumption is that "normal" data is generated by a gaussian process
		template <class T>
		class Detector1DU
		{
		public:
			double GetMu() {return mu;}
			double GetSigma() {return var;}
			Detector1DU(){mu = 0; var = 0; count = 0; M2 = 0;} 
			/// Train detector with the mean and standard deviation
			void Clear(){mu = 0; var = 0; count = 0; M2 = 0;}
			void Train(double mu_, double sigma_)
			{
				mu = mu_;
				var = sigma_*sigma_;
			}
			/// Updates the mean and variance for an additional element pt. Borrowed from http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
			void Update(T pt)
			{
				count++;
				double delta = pt - mu;
				mu = mu + delta/count;
				M2 += delta*(pt - mu);
				if(count > 1)
					var = M2/(count - 1 );
			}

			/// Train detector with the sampled data set
			void Train(const std::vector<T>& training_set)
			{
				int n = (int)training_set.size();
				mu = 0.0;
				var = 0.0;
				for (auto it = training_set.cbegin(); it!=training_set.end();++it)
				{
					mu += (double)(*it);
				}
				mu = mu / n;
				for (auto it = training_set.cbegin();it!=training_set.cend();++it)
				{
					var += (mu - (double)(*it))*(mu - (double)(*it));

				}
				var = var /(n-1);
			}
			/// Calculates the score of the outlier, the higher the score, the more likely point pt is an outlier
			double Score(T pt)
			{
				return std::abs((double)pt-mu);
			}
			/// Classifies point pt true = outlier, false = not an outlier 
			bool Classify(T pt)
			{
				//std::cout << "Abs : 3*sigma " <<  abs(mu - (double)pt) << " : " <<  3*sqrt(var) <<"\n";
				if (pt <= mu || (abs(mu - (double)pt) < 3*sqrt(var)))
				{
					return false;
				}
				else
				{
					return true;
				}
			}

		public:
			//for performance reasons variance to be used instead of standard deviation so that sqrt not calculated at every update
			double var;
			double mu;
			double count;
			double M2;
		};
	}
}

using namespace Detector_Components::Types;