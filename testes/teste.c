#include <stdio.h>
#include <hwloc.h>

int main()
{
        hwloc_topology_t t;
        
        hwloc_topology_init(&t);
        
        hwloc_topology_set_synthetic(t, "no:2 co:2 2");
        
        hwloc_topology_load(t);
	
	printf("%d\n\n", hwloc_topology_get_depth(t));
	
	return 0;
}