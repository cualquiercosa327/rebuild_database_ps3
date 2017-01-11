/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <ppu-lv2.h>

//syscall 379, CEX/DEX, root, sys_sm_shutdown:
//system_call_4(379,0x200,0,0,0); // 0x1100/0x100 = turn off,
//0x1200 = Hard Reboot, 0x200 = Soft Reboot,
//0x8201/0x8202 = load next/previous? lpar
//HV System Manager access - ServiceID 1 (REQUEST) 

char magic[] = {
	0x00, 0x00, 0x03, 0xE9
};

/*
 * lv2_sm_shutdown
 */
static inline int reboot(uint16_t op, const void *buf, uint64_t unk1, uint64_t size)
{
	lv2syscall4(379, op, (uint64_t) buf, unk1, size);
	return_to_user_prog(int);
}

//main
int main(int argc, char **argv)
{
	int result;
	
	printf("%s:%d: start\n", __func__, __LINE__);
	
	printf("%s:%d: writing file to hdd...\n", __func__, __LINE__);
	
	FILE *fp = fopen ("/dev_hdd0/mms/db.err", "wb");
	
	fwrite(magic, 1, 4, fp);
	
	fclose(fp);
	
	printf("%s:%d: file written, rebooting...\n", __func__, __LINE__);
	
	result = reboot(0x1200, 0, 0, 0);
	if (result) {
		printf("%s:%d: lv2_sm_shutdown failed (0x%08x)\n", __func__, __LINE__, result);
		goto done;
	}

	printf("%s:%d: end\n", __func__, __LINE__);

done:

	return 0;
}
