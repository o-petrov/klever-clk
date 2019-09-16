/*
 * Copyright (c) 2019 ISP RAS (http://www.ispras.ru)
 * Ivannikov Institute for System Programming of the Russian Academy of Sciences
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <linux/module.h>
#include <linux/clk.h>
#include <verifier/nondet.h>

static int __init ldv_init(void)
{
	struct device *dev = ldv_undef_ptr();
	const char *id = ldv_undef_ptr();
	struct clk *clk;

	/* clk_get */
	clk = clk_get(dev, id);
	if (IS_ERR(clk))
		clk = NULL;	
	if (clk) {
		if (clk_enable(clk) != 0)
			clk = NULL;
	}
	if (clk) {
		clk_disable(clk);
		clk_put(clk);
	}

	clk = clk_get(dev, id);
	if (IS_ERR(clk))
		clk = NULL;	
	if (clk) {
		if (clk_prepare(clk) != 0)
			clk = NULL;
	}
	if (clk) {
		clk_unprepare(clk);
		clk_put(clk);
	}

	clk = clk_get(dev, id);
	if (IS_ERR(clk))
		clk = NULL;		
	if (clk) {
		if (clk_prepare_enable(clk) != 0)
			clk = NULL;
	}
	if (clk) {
		clk_disable_unprepare(clk);
		clk_put(clk);
	}


	/* devm_clk_get */
	clk = devm_clk_get(dev, id);
	if (IS_ERR(clk))
		clk = NULL;	
	if (clk) {
		if (clk_enable(clk) != 0)
			clk = NULL;
	}
	if (clk) {
		clk_disable(clk);
		devm_clk_put(dev, clk);
	}

	clk = devm_clk_get(dev, id);
	if (IS_ERR(clk))
		clk = NULL;	
	if (clk) {
		if (clk_prepare(clk) != 0)
			clk = NULL;
	}
	if (clk) {
		clk_unprepare(clk);
		devm_clk_put(dev, clk);
	}

	clk = devm_clk_get(dev, id);
	if (IS_ERR(clk))
		clk = NULL;		
	if (clk) {
		if (clk_prepare_enable(clk) != 0)
			clk = NULL;
	}
	if (clk) {
		clk_disable_unprepare(clk);
		devm_clk_put(dev, clk);
	}

	return 0;
}

module_init(ldv_init);
