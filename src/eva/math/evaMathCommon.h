#ifndef EVAMATHCOMMON_H_
#define EVAMATHCOMMON_H_

namespace eva
{
	class MathCommon
	{
		public:
			template <class T>
			static bool isWithin(T first, T second, T val)
			{
				if(first > second)
				{
					if(val >= second && val <= first)
						return true;
				}
				else
				{
					if(val <= second && val >= first)
						return true;
				}
				return false;
			}
		private:
			MathCommon(){};
	};
};

#endif /* EVAMATHCOMMON_H_ */
