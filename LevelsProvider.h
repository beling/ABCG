#ifndef LEVELSPROVIDER_H_
#define LEVELSPROVIDER_H_

#include <string>
#include <vector>

class Episode {
	
	public:
		
	std::vector<std::string> levels;
	
	std::string name;
	
	Episode(std::string name): name(name) {}
	
	std::string level(int nr) { return levels[nr]; }
	
	const std::string level(int nr) const { return levels[nr]; }
	
	int levels_len() const { return levels.size(); }
	
	bool operator<(const Episode& other) const {
		return name < other.name;
	}
	
};

class LevelsProvider {
	
	std::vector<Episode> episodes;
	
public:
	
	LevelsProvider();
	
	///@return episode number for level with given id
	int episode_with_id(int id) const { return id >> 16; }
	
	///@return level number for level with given id
	int level_with_id(int id) const { return id & ((1 << 16)-1); }
	
	///@return unique level id
	int id(int episode_nr, int level_nr) const { return (episode_nr << 16) + level_nr; }
	
	void scan();
	
	Episode& episode(int nr) { return episodes[nr]; }
	
	const Episode& episode(int nr) const { return episodes[nr]; }
	
	int episodes_len() const { return episodes.size(); }
	
	bool level_exist(int episode_nr, int level_nr) const;
	
	bool level_exist(int id) const { return level_exist(episode_with_id(id), level_with_id(id)); }
	
	int next_level_id(int episode_nr, int level_nr) const;
	
	int next_level_id(int id) const { return next_level_id(episode_with_id(id), level_with_id(id)); }
	
	int prev_level_id(int episode_nr, int level_nr) const;
	
	int prev_level_id(int id) const { return prev_level_id(episode_with_id(id), level_with_id(id)); }
	
	std::string file_name(int episode_nr, int level_nr) const;
	
	std::string file_name(int id) const { return file_name(episode_with_id(id), level_with_id(id)); }
};

#endif /*LEVELSPROVIDER_H_*/
