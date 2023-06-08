int
sys_pgcnt(void)
{
    pde_t *pde;  // page directory
    pte_t *pte;  // page table

    int i = 0;
    int j = 0;
    int cnt = 0;

    unsigned int flag = PTE_P | PTE_U;

    for(i=0; i < NPDENTRIES; i++)        
        pde = &((myproc()->pgdir)[i]);

        if(*pde & PTE_P) {            
            pte = (pte_t*) P2V(PTE_ADDR(*pde));
            
            for(j=0; j < NPTENTRIES; j++) {                
                if((pte[j] & flag) == flag) {                   
                   cnt = cnt + 1;
                }
            }
        }  
    }

    return cnt;
}