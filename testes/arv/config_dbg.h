#ifndef CONFIG_DBG_H
#define CONFIG_DBG_H

struct ConfigDebug
{
	struct Arvore
	{
		static const bool DEBUG = false;
		static const int DEBUG_MAX_LINHAS_IDS = 100;
	};
};

#endif /* CONFIG_DBG_H */
