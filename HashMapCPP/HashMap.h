#include <memory>
#include <vector>

template<typename TKey, typename TValue>
class HashMap
{
private:
	std::vector<std::vector<std::pair<TKey, TValue>>> Data;
	std::hash<TKey> Hash;
	void ReHash();
	double MaxLoadFactor;
	std::vector<TKey> Keys;
public:
	HashMap(int startingLength, double maxLoadFactor = 0.75);
	static std::unique_ptr<HashMap<TKey,TValue>> Create(int startingLength, double maxLoadFactor = 0.75);
	bool Add(TKey key,TValue value);
	bool Remove(TKey key);
	bool Contains(TKey key) const;
	std::unique_ptr<TValue> Find(TKey key) const;
	int Count;
};

template<typename TKey, typename TValue>
HashMap<TKey,TValue>::HashMap(int startingLength, double maxLoadFactor)
{
	Data = std::vector<std::vector<std::pair<TKey, TValue>>>(startingLength);
	/*for (int i = 0; i < startingLength; i++)
	{
		Data.emplace_back();
	}*/
	Hash = std::hash<TKey>{};
	Count = 0;
	MaxLoadFactor = maxLoadFactor;
	Keys = std::vector<TKey>();
}

template<typename TKey, typename TValue>
static std::unique_ptr<HashMap<TKey,TValue>> HashMap<TKey, TValue>::Create(int startingLength, double maxLoadFactor)
{
	return std::make_unique<HashMap<TKey, TValue>>(startingLength);
}

template<typename TKey, typename TValue>
bool HashMap<TKey, TValue>::Add(TKey key, TValue value)
{
	int bucketIndex = Hash(key) % Data.size();
	for (int i = 0; i < Data[bucketIndex].size(); i++)
	{
		if (Data[bucketIndex][i].first == key) return false;
	}
	//Data[bucketIndex].push_back(std::make_pair<TKey, TValue>(key, value));
	Data[bucketIndex].emplace_back(key, value);
	Keys.push_back(key);
	Count++;
	if ((double)Count/Data.size() > MaxLoadFactor) ReHash();
	return true;
}

template<typename TKey, typename TValue>
void HashMap<TKey,TValue>::ReHash()
{
	std::vector<std::vector<std::pair<TKey,TValue>>> newMap = std::vector<std::vector<std::pair<TKey, TValue>>>{ Data.size()*2 };
	for (int i = 0; i < Keys.size(); i++)
	{
		int newBucketIndex = Hash(Keys[i]) % newMap.size();
		int oldBucketIndex = Hash(Keys[i]) % Data.size();
		for (auto pair : Data[oldBucketIndex])
		{
			if (pair.first == Keys[i])
			{
				newMap[newBucketIndex].emplace_back(Keys[i], pair.second);
				break;
			}
		}
	}
	Data = std::move(newMap);
}

template<typename TKey,typename TValue>
bool HashMap<TKey, TValue>::Remove(TKey key)
{
	int bucketIndex = Hash(key) % Data.size();
	bool didFind = false;
	for (int i = 0; i < Data[bucketIndex].size(); i++)
	{
		if (Data[bucketIndex][i].first == key)
		{
			Data[bucketIndex].erase(std::find(Data[bucketIndex].begin(),Data[bucketIndex].end(),Data[bucketIndex][i]));
			didFind = true;;
		}
	}
	if (!didFind) return false;
	Keys.erase(std::find(Keys.begin(), Keys.end(), key));
	Count--;
	return true;
}

template<typename TKey,typename TValue>
bool HashMap<TKey, TValue>::Contains(TKey key) const
{
	auto iterator = std::find(Keys.begin(), Keys.end(), key);
	if (iterator == Keys.end()) return false;
	return true;
}

template<typename TKey, typename TValue>
std::unique_ptr<TValue> HashMap<TKey, TValue>::Find(TKey key) const
{
	int bucketIndex = Hash(key) % Data.size();
	for (auto pair : Data[bucketIndex])
	{
		if (pair.first == key) return std::make_unique<TValue>(pair.second);
	}
	return nullptr;
}

//template <typename T>
//int IndexOf(std::vector<T>& vec, T& item)
//{
//	auto itr = std::find(vec.begin(), vec.end(), item);
//
//	if (itr != vec.end())
//	{
//		// vec.erase(itr);
//		return std::distance(vec.begin(), itr);
//	}
//
//	return -1;
//}