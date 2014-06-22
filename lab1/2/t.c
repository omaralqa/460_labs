#include "ext2.h"  /* download this from LAB1 */
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

#define BLK 1024

GD    *gp;
INODE *ip;
DIR   *dp;

int prints(char *s)
{
   while (*s)
   {
      putc(*s++);
   }

}

u16 getblk(u16 blk, char *buf){
   u16 cyl, head, sector;
   cyl = (2 * blk) / 36;
   head = ((2 * blk)%36)/18;
   sector = ((2*blk)%36) % 18;
   readfd(cyl, head, sector, buf);
}

main()
{ 
  u16  i, k, iblk, inode;
  char buf1[BLK], buf2[BLK], buf3[64], *cp;

  prints("booter start\n\r");  
  getblk((u16)2, buf1);
  
  gp = (GD *)buf1;
  iblk = (u16)gp->bg_inode_table;
  prints("inode_block="); putc(iblk+'0');
  getblk(iblk, buf1);
  
  ip = (INODE *)buf1 + 1;
  for (k = 0; k < 12; k++)
  {
      prints("\n\rBlock# "); putc(k+'0'); prints(" -> ");
      if ((u16)ip->i_block[k] != 0){ // Read each DIRECT block if it contains data
         
         getblk((u16)ip->i_block[k], buf2); 
         dp = (DIR *)buf2;
         cp = buf2;
         while (cp < buf2 + 1024){
            for (i = 0; i < dp->name_len; i++){
               putc(dp->name[i]);
            }
            putc(' ');
            strncpy(buf3, dp->name, dp->name_len);
            
            cp += dp->rec_len;
            dp = (DIR *)cp;
      } 
    }
  }
}
 
