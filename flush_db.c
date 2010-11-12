#include	<stdio.h>
#include	<stdlib.h>
#include	<sqlite3.h>

int main(int argc, const char *argv[])
{
	sqlite3 *handle;
	sqlite3_stmt *stmt;
	sqlite3_open("chafing.db", &handle);

sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(1	,'三鲜锅底',	6,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(2	,'大三鲜',	8,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(3	,'全辣锅底',	10,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(4	,'大全辣',	12,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(5	,'鸳鸯火锅',	8,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(6	,'大鸳鸯',	10,0,100,0)",0,0,0);

sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(101	,'大白菜',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(102	,'包菜',	1,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(103	,'青菜',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(104	,'黄豆芽',	1,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(105	,'绿豆芽',	0.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(106	,'土豆',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(107	,'平菇',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(108	,'冬瓜',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(109	,'生菜',	2,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(110	,'白萝卜',	1,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(111	,'花菜',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(112	,'山药',	3,0,100,0)",0,0,0);

sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(201	,'豆腐果',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(202	,'千张结',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(203	,'素鸡',	2,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(204	,'宽粉皮',	2,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(205	,'豆腐皮',	2.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(206	,'冻豆腐',	2,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(207	,'干丝',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(208	,'日本豆腐',	1.5,0,100,0)",0,0,0);

sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(301	,'海带',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(302	,'面筋果',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(303	,'粉丝',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(304	,'粉条',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(305	,'木耳',	2,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(306	,'皮肚',	2.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(307	,'香菇',	2.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(308	,'腐竹',	2,0,100,0)",0,0,0);

sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(401	,'生鸡蛋',	1,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(402	,'鹌鹑蛋',	3,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(403	,'鸭血',	2,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(404	,'火腿肠',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(405	,'金针菇',	3,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(406	,'笋片',	2.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(407	,'香肠',	4.5,0,100,0)",0,0,0);

sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(501	,'鸡蛋面',	2,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(502	,'老油条',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(503	,'黄金小馒头',	2.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(504	,'锅巴',	2.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(505	,'黄金南瓜饼',	2.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(506	,'水饺',	3,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(507	,'薯条',	3.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(508	,'年糕',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(509	,'米饭',	1,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(510	,'脆皮香蕉',	2.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(511	,'香芋地瓜丸',	2.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(512	,'紫薯豌豆丸',	2.5,0,100,0)",0,0,0);

sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(601	,'小黄鱼',	3,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(602	,'蟹柳',	3,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(603	,'鲳鳊鱼',	4,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(604	,'甜虾',	4,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(605	,'鱿鱼',	4,0,100,0)",0,0,0);

sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(700	,'亲亲肠',	3.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(701	,'甜不辣',	4,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(702	,'猪肉圆',	3.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(703	,'牛肉圆',	3.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(704	,'撒尿牛丸',	4.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(705	,'香菇肉圆',	3.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(706	,'鱼圆',	3.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(707	,'虾圆',	3.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(708	,'北极翅',	3.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(709	,'午餐肉',	5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(710	,'蛋饺',	4,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(711	,'千层卷',	3.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(712	,'大红肠',	5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(713	,'腰片',	5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(714	,'肚丝',	5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(715	,'鸭肠',	3,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(716	,'鸭心',	3,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(717	,'鸭舌',	4,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(718	,'鸡翅尖',	3.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(719	,'鸭肫',	4,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(720	,'凤爪',	4.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(721	,'牛肚丝',	5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(722	,'牛肉卷',	8,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(723	,'新疆羊肉',	8,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(724	,'新西兰羊肉',	10,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(725	,'里脊肉',	5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(726	,'脆皮包芯丸',	4,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(727	,'田鸡丸',	4,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(728	,'包心虾丸',	4.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(729	,'包心鱼丸',	4.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(730	,'鱼豆腐',	3.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(731	,'大份牛肉卷',	15,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(732	,'大份新疆羊肉',15,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(733	,'大份新西兰羊肉', 	20,0,100,0)",0,0,0);

sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(801	,'雪花',	2.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(802	,'雪花冰鲜',	3,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(803	,'雪花勇闯天涯',5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(804	,'哈尔滨冰啤',  3,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(805	,'听可乐/雪碧',	2.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(806	,'600ml可乐/雪碧',	3,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(807	,'1.25l可乐/雪碧',	5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(808	,'2l百事/雪碧',	6.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(809	,'2l美年达',	6.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(810	,'450ml果粒橙',	3,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(811	,'1.25l果粒橙',	7.0,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(812	,'康师傅冰红茶',3,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(813	,'1.5l康师傅冰茶',6,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(814	,'王老吉',	4,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(815	,'银鹭椰子汁',	2.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(816	,'椰树椰子汁',	3,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(817	,'营养快线',	4,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(818	,'1.5l营养快线',10,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(819	,'旺仔牛奶',	4,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(820	,'康师傅矿泉水',1,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(821	,'小劲酒',	9,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(822	,'沙洲优黄',	11,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(823	,'迎驾陈窖',	21,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(824	,'小迎驾',	11,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(825	,'红高沟',	15,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(826	,'三星迎驾',	48,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(827	,'二锅头',	2.5,0,100,0)",0,0,0);

sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(901	,'芝麻酱',	2.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(902	,'四季宝花生酱',	2.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(903	,'四川辣酱',	2,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(904	,'韩国辣酱',	2,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(905	,'豆腐乳',	1,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(906	,'香油蒜泥',	2,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(907	,'沙茶酱',	3,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(908	,'蒜蓉辣酱',	1.5,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(909	,'番茄酱',	1,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(910	,'甜面酱',	1,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(911	,'清洁餐具',	1,0,100,0)",0,0,0);
sqlite3_exec(handle,"insert into menu (id,name,price,cost,stocks,acc) values(912	,'川崎酱',	2,0,100,0)",0,0,0);

	sqlite3_close(handle);
	return 0;
}
