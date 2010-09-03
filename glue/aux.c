/*
 * =====================================================================================
 *
 *       Filename:  aux.c
 *
 *    Description:  
 *
 *        Created:  20.08.10
 *       Revision:  
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<errno.h>
#include	"event.h"
#include	"glue.h"

static int sql_item_cnt;
static pthread_t pt[PTHREAD_NUM];


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_date_time
 *  Description:  date changes 1 time per day, so make it long live
 * =====================================================================================
 */
char *get_date_time(int req)
{
	static char date[11];	/* 10 + '\0' */
	static char wday[11];
	static char hm[6];	/* 5 + '\0' */

	if (req == GET_DATE && *date) {		/* date will always be true, *date */
		return date;
	}
	if (req == GET_WDAY && *wday) {
		return wday;
	}

	/*-----------------------------------------------------------------------------
	 *  need time or haven't init
	 *-----------------------------------------------------------------------------*/
	time_t timep; 
	struct tm *p; 
	time(&timep); 
	p = gmtime(&timep); 

	switch (req) {
	case GET_DATE:
		sprintf(date, "%d/%.2d/%.2d",
			p->tm_year + 1900,
			p->tm_mon + 1,
			p->tm_mday);
		return date;
	case GET_TIME:
		sprintf(hm, "%.2d:%.2d",
			p->tm_hour,
			p->tm_min);
		return hm;
	case GET_WDAY:
		switch (p->tm_wday) {
		case 0:
			sprintf(wday, "周日");
			break;
		case 1 ... 6:
			sprintf(wday, "周%d", p->tm_wday);
			break;
		default:
			break;
		}
		return wday;
	default:
		break;
	}
}

void set_sql_item_cnt(int n)
{
	sql_item_cnt = n;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_sql_item_cnt
 *  Description:  called after get_bill_list
 *
 *  get_bill_list -> db_select -> set_sql_item_cnt
 * =====================================================================================
 */
int get_sql_item_cnt()
{
	return sql_item_cnt;
}

char **bulk_space(int cnt, int size)
{
	/* bulk */
	char **res = calloc(cnt, size);
	int str_start = (int)((char *)res + str_offset(cnt));
	int str_addr = str_start;

	/* retail */
	for (int i = 0; i < cnt; ++i) {
		res[i] = (char *)str_addr;
		str_addr += (size - PTR_SIZE);
	}
	return res;
}

char **bulk_expand(char **res, int cnt_orig, int size, int cnt)
{
	int ptrs = str_offset(cnt_orig);
	char **res_large = bulk_space(cnt_orig + cnt, size);
	memcpy(res_large, res, ptrs);
	memcpy((char *)res_large + str_offset(cnt_orig + cnt), 
	       (char *)res       + ptrs, 
	        cnt_orig * (size - PTR_SIZE));
	return res_large;
}

void *spc2zr(char *str)
{
	int ch;
	while ((ch = *str) != '\0') {
		if (ch == 0x20) {
			*str = '\0';
			return NULL;
		} else {
			++str;
		}
	}
	return NULL;
}

char *strdelim(char *str, int ch, char **saveptr)
{
	int i = 0;
	while (*str != '\0') {
		if (*str == ch) {
			*str++ = '\0';
			saveptr[i++] = str;
		} else {
			++str;
		}
	}
	return str;
}

void *list2file(char **list)
{
	int i = 0;
	char path[50];
	char date[11];
	char time[6];
	strncpy(date, get_date_time(GET_DATE), 10);
	strncpy(time, get_date_time(GET_TIME), 5);
	date[10] = '\0';
	time[5] = '\0';
	time[2] = '_';		/* 15:54 -> 15_54 */

	/* open the file */
	sprintf(path, "ch%s.txt", time);
	FILE *fp = fopen(path, "a");
	fprintf(stderr, "%d\r\n", errno);
	perror("fopen");

	/* output */
	time[2] = ':';		/* 15_54 -> 15:54 */
	fprintf(fp, "感谢赏脸吴哥火锅\n%s  %s\n\n", date, time);
	while (*list[i]) {
		fprintf(fp, "%s\n", list[i++]);
	}
	fprintf(fp, "\n折扣：%5.1f\n", get_discount());
	fprintf(fp,   "合计：%5.1f\n", get_price_total());
	fprintf(fp,   "实收：%5.1f\n", get_receive_total());
	fprintf(fp,   "找零：%5.1f\n", get_change());
	fclose (fp);
	return NULL;
}

pthread_t *get_pthread_t(int idx)
{
	return &pt[idx];
}
