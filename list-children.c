#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
MODULE_DESCRIPTION("List forking children");
MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");
void print_children(struct task_struct *p);

static int my_proc_init(void)
{
struct task_struct *p; /* Needed for later **/
printk("Current process: pid = %d; name = %s\n",
current ->pid , current ->comm);
printk("\nProcess list:\n\n");
for_each_process(p){
    if(strcmp(p->comm,"forking")){
        printk("parent forking pid = %d\n",p->pid);
        print_children(p);
        break;
    }
}

return 0;
}
void print_children(struct task_struct *p){
    struct task_struct *child;
    struct head_list *list=&(p->children);
    list_for_each_entry(child,list,sibling){
        printk("child pid = %d\n",child->pid);
    }
}
static void my_proc_exit(void)
{
printk("Current process: pid = %d; name = %s\n",
current ->pid , current ->comm);
}
module_init(my_proc_init);
module_exit(my_proc_exit);