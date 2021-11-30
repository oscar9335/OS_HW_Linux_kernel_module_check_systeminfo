#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/proc_fs.h>
#include <linux/fs.h>

#include <linux/seq_file.h>

#include <linux/utsname.h>
#include <linux/uts.h>

#include <linux/mm.h>

#include <linux/time.h>
#include <linux/kernel_stat.h>

#include <linux/cpufreq.h>
#include <asm/processor.h>

struct sysinfo i;
struct timespec uptime;
struct timespec idle;
struct cpuinfo_x86 *c;
unsigned long pages[NR_LRU_LISTS];
int lru;
int t;
int k = 0;
u64 nsec;
u32 rem;

unsigned int cpu;
int cp;


static int my_info_show(struct seq_file *m, void *v)
{
    /*************************version**************************************/
    seq_printf(m,"\n============Version============\n");
    seq_printf(m, "Linux version %s\n",utsname()->release);

    /*************************CPU****************************************/
    seq_printf(m,"\n============CPU=================\n");
    for(k = 0 ; k < nr_cpu_ids ; k++)
    {
        c = &cpu_data(k);
        cpu = c-> cpu_index;
        seq_printf(m, "processor\t: %u\n",cpu);
        seq_printf(m, "model name\t: %s\n", c->x86_model_id[0] ? c->x86_model_id : "unknown");
        seq_printf(m, "physical id\t: %d\n", c->phys_proc_id);
        seq_printf(m, "core id\t\t: %d\n", c->cpu_core_id);
        seq_printf(m, "cpu cores\t: %d\n", c->booted_cores);
        seq_printf(m, "cache size\t: %d KB\n", c->x86_cache_size);
        seq_printf(m, "clflush size\t: %u\n", c->x86_clflush_size);
        seq_printf(m, "cache_alignment\t: %d\n", c->x86_cache_alignment);
        seq_printf(m, "address sizes\t: %u bits physical, %u bits virtual\n",c->x86_phys_bits, c->x86_virt_bits);
        seq_printf(m,"\n");
    }

    /*************************Memory*************************************/
    seq_printf(m,"\n============Memory=============\n");
    si_meminfo(&i);

    for (lru = LRU_BASE; lru < NR_LRU_LISTS; lru++)
        pages[lru] = global_node_page_state(NR_LRU_BASE + lru);

    seq_printf(m, "MemTotal	:%lukb\n", i.totalram);
    seq_printf(m, "MemFree		:%lukb\n", i.freeram);
    seq_printf(m, "Buffers		:%lukb\n", i.bufferram);
    seq_printf(m, "Active		:%lukb\n", pages[LRU_ACTIVE_ANON] + pages[LRU_ACTIVE_FILE]);
    seq_printf(m, "Inactive	:%lukb\n", pages[LRU_INACTIVE_ANON] + pages[LRU_INACTIVE_FILE]);
    seq_printf(m, "Shmem		:%lukb\n", i.sharedram);
    seq_printf(m, "Dirty		:%lukb\n",global_node_page_state(NR_FILE_DIRTY));
    seq_printf(m, "Writeback	:%lukb\n",global_node_page_state(NR_WRITEBACK));
    seq_printf(m, "KernelStack	:%lukB\n",global_zone_page_state(NR_KERNEL_STACK_KB));
    seq_printf(m, "PageTables	:%lukb\n",global_zone_page_state(NR_PAGETABLE));

    /**********************************Time***************************************/

    seq_printf(m,"\n============Time==============\n");

    nsec = 0;
    for_each_possible_cpu(t)
    nsec += (__force u64) kcpustat_cpu(t).cpustat[CPUTIME_IDLE];

    get_monotonic_boottime(&uptime);

    idle.tv_sec = div_u64_rem(nsec, NSEC_PER_SEC, &rem);
    idle.tv_nsec = rem;


    seq_printf(m, "Uptime		:%lu.%02lu(s)\n",(unsigned long) uptime.tv_sec,(uptime.tv_nsec / (NSEC_PER_SEC / 100)));
    seq_printf(m, "Idletime	:%lu.%02lu(s)\n",(unsigned long) idle.tv_sec,(idle.tv_nsec / (NSEC_PER_SEC / 100)));

    return 0;
}



static int my_info_open(struct inode *inode, struct  file *file)
{
    return single_open(file, my_info_show, NULL);
}

static const struct file_operations my_info_fops =
{
    .owner = THIS_MODULE,
    .open = my_info_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

static int __init my_info_init(void)
{
    proc_create("my_info", 0, NULL, &my_info_fops);
    return 0;
}

static void __exit my_info_exit(void)
{
    remove_proc_entry("my_info", NULL);
}

module_init(my_info_init);
module_exit(my_info_exit);
MODULE_LICENSE("GPL");
