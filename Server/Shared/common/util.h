#ifndef __TWP_UTIL_UTIL_HEADER__
#define __TWP_UTIL_UTIL_HEADER__

template<typename Tint>
inline Tint RangedRand(Tint nMin, Tint nMax)
{
	return nMin + (float32)rand() / (RAND_MAX + 1) * (nMax - nMin);
}

namespace twp
{
	namespace common
	{
		namespace utility
		{

			inline int CalcGroupCount(int count, int max_pile_count)
			{
				if (max_pile_count == 0)
				{
					return 1;
				}
				int group_count = count / max_pile_count;
				if (count % max_pile_count != 0)
				{
					++group_count;
				}
				return group_count;
			}

			template <typename MapType>
			inline void AddValueInMap(MapType& map_obj, typename MapType::key_type key, typename MapType::mapped_type value)
			{
				typename MapType::iterator iter = map_obj.find(key);
				if (iter != map_obj.end())
				{
					iter->second += value;
				}
				else
				{
					map_obj.insert(std::make_pair(key, value));
				}
			}

			template <typename MapType>
			inline typename MapType::mapped_type* GetValPtrFromMap(MapType& map_obj, typename MapType::key_type& key)
			{
				typename MapType::iterator iter = map_obj.find(key);
				return iter == map_obj.end() ? NULL : &iter->second;
			}

			template <typename MapType>
			inline bool InsertToMap(MapType& map_obj, typename MapType::key_type& key, typename MapType::mapped_type& value)
			{
				typename MapType::iterator iter = map_obj.find(key);
				if (iter != map_obj.end())
					return false;
				map_obj.insert(std::make_pair(key, value));
				return true;
			}


			// 值类型一定为指针
			template <typename MapType>
			inline typename MapType::mapped_type GetValFromMap(MapType& map_obj, typename MapType::key_type key)
			{
				typename MapType::iterator iter = map_obj.find(key);
				return iter == map_obj.end() ? NULL : iter->second;
			}

			template <typename T, unsigned int count>
			inline unsigned int FindFirstNotOf(const T (&arr)[count], const T& elem)
			{
				unsigned int index = 0;
				for (unsigned int i = 0; i < count; ++i)
				{
					if (elem != arr[i])
					{
						index = i;
						break;
					}
				}
				return index;
			}

			template <typename T, unsigned int count>
			inline unsigned int FindFirstOf(const T (&arr)[count], const T& elem)
			{
				unsigned int index = 0;
				for (unsigned int i = 0; i < count; ++i)
				{
					if (elem == arr[i])
					{
						index = i;
						break;
					}
				}
				return index;
			}

		}
	}
}

#ifdef TWP_SNPRINTF
# undef	TWP_SNPRINTF
#endif // TWP_SNPRINTF
#if defined(linux) || defined(__linux) || defined(__linux__)
#  define TWP_SNPRINTF snprintf
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#  define TWP_SNPRINTF(str, size, fmt, ...) _snprintf_s(str, size, (size) - 1, fmt, ##__VA_ARGS__)
#endif


#endif