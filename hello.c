// SPDX-License-Identifier: GPL-2.0+

/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/slab.h>

#include <linux/ktime.h>

struct personal_list_head {
struct personal_list_head *next;
ktime_t time;
};

MODULE_AUTHOR("Kramar Viktor IO-83");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static struct personal_list_head *header;


static uint count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "Number of messages Hello, world");


static int __init hello_init(void)
{
	uint i = 0;
	struct personal_list_head *first, *second;

	if (count == 0) {
		pr_warn("WARNING!\nCounter = 0");
	} else if (count >= 5 && count <= 10) {
		pr_warn("WARNING!\nCounter between 5 and 10");
	} else if (count > 10) {
		pr_err("Invalid value!");
		return -EINVAL;
	}

	header = kmalloc(sizeof(struct personal_list_head *), GFP_KERNEL);
	first = header;
	for (i = 0; i < count; i++) {
		first->next = kmalloc(sizeof(struct personal_list_head), GFP_KERNEL);
		first->time = ktime_get();
		pr_info("Hello World!");
		second = first;
		first = first->next;
	}
	if (count != 0) {
		kfree(second->next);
		second->next = NULL;
	}
	pr_info("count: %d\n", count);
	return 0;
}


static void __exit hello_exit(void)
{
	struct personal_list_head *variable;
	while (header != NULL && count != 0) {
		variable = header;
		pr_info("Time : %lld", variable->time);
		header = variable->next;
		kfree(variable);
	}
	if (count == 0)
		pr_info("Time is 0 (print never used)");
		pr_info("");
}

module_init(hello_init);
module_exit(hello_exit);
