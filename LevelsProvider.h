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
	
};

class LevelsProvider {
	
	std::vector<Episode> episodes;
	
public:
	
	LevelsProvider();
	
	void scan();
	
	Episode& episode(int nr) { return episodes[nr]; }
	
	const Episode& episode(int nr) const { return episodes[nr]; }
	
	int episodes_len() const { return episodes.size(); }
	
	int id(int episode_nr, int level_nr) const { return (episode_nr << 16) + level_nr; }
	
	std::string file_name(int episode_nr, int level_nr) const;
	
	std::string file_name(int id) const { return file_name(id >> 16, id & ((1 << 16)-1)); }
};

#endif /*LEVELSPROVIDER_H_*/
