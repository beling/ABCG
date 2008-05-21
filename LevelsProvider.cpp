#include "LevelsProvider.h"

#include "libs/osdir.h"

#ifndef LEVELS_DIRECORY
#define LEVELS_DIRECORY "." PATH_SEPARATOR "levels"
#endif

void LevelsProvider::scan() {
	episodes.clear();
	Episode* current;
	oslink::directory epis(LEVELS_DIRECORY);
	while (epis) {
		std::string episode_name = epis.next();
		if (episode_name == "." || episode_name == "..") continue;
		oslink::directory level(std::string(LEVELS_DIRECORY) + PATH_SEPARATOR + episode_name);
		current = 0;
		while (level) {
			std::string level_name = level.next();
			if (level_name == "." || level_name == "..") continue;
			if (!current) {
				episodes.push_back(Episode(episode_name));
				current = &episodes.back();
			}
			current->levels.push_back(level_name);
		}
	}
}

LevelsProvider::LevelsProvider()
{
	scan();
}

std::string LevelsProvider::file_name(int episode_nr, int level_nr) const {
	return std::string(LEVELS_DIRECORY) + PATH_SEPARATOR +
		   episode(episode_nr).name + PATH_SEPARATOR +
		   episode(episode_nr).level(level_nr);
}

