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
	if (IS_ERR(clk) < 0)
		goto c2;
		
	if (clk_enable(clk) < 0)
		goto c1put;
	
	clk_disable(clk);
c1put:
	clk_put(clk);

c2:
	clk = clk_get(dev, id);
	if (IS_ERR(clk) < 0)
		goto c3;
		
	if (clk_prepare(clk) < 0)
		goto c2put;
	
	clk_unprepare(clk);
c2put:
	clk_put(clk);

c3:
	clk = clk_get(dev, id);
	if (IS_ERR(clk) < 0)
		goto c4;
		
	if (clk_prepare_enable(clk) < 0)
		goto c3put;
	
	clk_disable_unprepare(clk);
c3put:
	clk_put(clk);

	/* devm_clk_get */
c4:
	clk = devm_clk_get(dev, id);
	if (IS_ERR(clk) < 0)
		goto c5;
		
	if (clk_enable(clk) < 0)
		goto c4put;
	
	clk_disable(clk);
c4put:
	devm_clk_put(dev, clk);

c5:
	clk = devm_clk_get(dev, id);
	if (IS_ERR(clk) < 0)
		goto c6;
	
	if (clk_prepare(clk) < 0)
		goto c5put;

	clk_unprepare(clk);
c5put:
	devm_clk_put(dev, clk);

c6:
	clk = devm_clk_get(dev, id);
	if (IS_ERR(clk) < 0)
		goto ret;
	
	if (clk_prepare_enable(clk) < 0)
		goto c6put;
	
	clk_disable_unprepare(clk);
c6put:
	devm_clk_put(dev, clk);
ret:
	return 0;
}

module_init(ldv_init);
