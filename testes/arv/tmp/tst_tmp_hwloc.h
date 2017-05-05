/*
 * Classe para teste de tempo de ancestral comum usando hwloc
 */

#ifndef TST_TMP_HWLOC_H
#define TST_TMP_HWLOC_H

#include "tst_tmp.h"
#include <hwloc.h>
#include <string>

// Teste usando hwloc
class TesteAncestralComumHwloc : public BaseTesteAncestralComum<hwloc_topology_t, hwloc_obj_t>
{
	public:
	
	TesteAncestralComumHwloc(int nI, int nIA) : BaseTesteAncestralComum(nI, nIA) {};
	
	hwloc_topology_t criarArvore(std::vector<int> graus)
	{
		// Descrição da topologia sintética
		std::string s;
		int ultimo = graus.back();
		graus.pop_back();
		for (int g : graus)
			s += "ca:" + std::to_string(g) + ' '; // Caches evitam "compactação"
		s += std::to_string(ultimo);
		
		// Cria a topologia
		hwloc_topology_t t;
		hwloc_topology_init(&t);
		hwloc_topology_set_synthetic(t, s.data());
		hwloc_topology_load(t);
		
		return t;
	}
	
	std::vector<hwloc_obj_t> pegarFolhas(hwloc_topology_t t)
	{
		int ultNivel = hwloc_topology_get_depth(t) - 1;
		std::vector<hwloc_obj_t> folhas;
		hwloc_obj_t folha = hwloc_get_obj_by_depth(t, ultNivel, 0);
		while (folha != nullptr)
		{
			folhas.push_back(folha);
			folha = hwloc_get_next_obj_by_depth(t, ultNivel, folha);
		}
		return folhas;
	}
	
	hwloc_obj_t ancestralComum(hwloc_topology_t t, hwloc_obj_t a, hwloc_obj_t b)
	{
		return hwloc_get_common_ancestor_obj(t, a, b);
	}
	
	void destruirArvore(hwloc_topology_t t)
	{
		hwloc_topology_destroy(t);
	}
};

#endif /* TST_TMP_HWLOC_H */
