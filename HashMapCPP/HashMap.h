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
	bool Contains(TKey key);
	TValue Find(TKey key);
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
	//Data[bucketIndex].emplace_back(std::make_pair<TKey, TValue>(key, value));
	Data[bucketIndex].emplace_back({ key, value });
	Keys.push_back(key);
	Count++;
	if ((double)Count/Data.size() > MaxLoadFactor) ReHash();
	return true;
}

template<typename TKey, typename TValue>
void HashMap<TKey,TValue>::ReHash()
{
	auto newMap = std::vector<std::vector<std::pair<TKey, TValue>>>{ Data.size()*2 };
	for (int i = 0; i < Keys.size(); i++)
	{
		// get the new bucket index
		// go to that bucket index and push_back
		newMap[Hash(Keys[i]) % newMap.size()].push_back({ Keys[i],Data[Hash(Keys[i]) % Data.size()][Keys[i]]});
	}
	Data = std::move(newMap);
}

