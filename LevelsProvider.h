/*
    ABCG (Another Bridge Construct Game)
    Copyright (C) 2010  Piotr Beling

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LEVELSPROVIDER_H_
#define LEVELSPROVIDER_H_

#include <string>
#include <vector>

/**
 * Represent episode, which is named vector of levels.
 */
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

/**
 * Provides access to levels. Allow reading them from files.
 * Scan LEVELS_DIRECORY for episodes (subdirectories), and each episode directory for levels (files)
 * LEVELS_DIRECORY should be defined as symbol, or it will be have default value: "." PATH_SEPARATOR "levels"
 */
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
