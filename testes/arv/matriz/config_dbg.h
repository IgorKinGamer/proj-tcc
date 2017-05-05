#ifndef MATRIZ_CONFIG_DBG_H
#define MATRIZ_CONFIG_DBG_H

namespace matriz
{
	struct ConfigDebug
	{
		struct No
		{
			static const int PREENCHIMENTO = 100;
		};
		
		struct Arvore
		{
			static const bool DEBUG = false;
			static const int DEBUG_MAX_LINHAS_IDS = 0;
		};
	};
}

#endif /* MATRIZ_CONFIG_DBG_H */
