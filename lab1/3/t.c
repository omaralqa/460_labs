#include "ext2.h"

typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

#define BLK 1024
#define INODES_PER_BLOCK (BLK/sizeof(INODE))

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

//int gets(char s[])
//{
//   char a;
//   int i = 0, j;
//   a = getc();
//   
//   while (a != '\r'){
//      s[i]=a;
//      a=getc();
//      i++;
//   }
//   s[i]='\n';
//   return s;
//}

u16 getblk(u16 blk, char *buf){
   u16 cyl, head, sector;
   cyl = (2 * blk) / 36;
   head = ((2 * blk)%36)/18;
   sector = ((2*blk)%36) % 18;
   readfd(cyl, head, sector, buf);
}

u16 search (u16 inode, char filename[]){
    
    char buf1[BLK], buf2[BLK], buf3[10], *cp;
    u16 iblk, k, i;
    
    iblk = ((inode - 1) / INODES_PER_BLOCK + 5);
    getblk((u16) iblk, buf1);
    ip = (INODE *)buf1 + (u16)((inode-1) % INODES_PER_BLOCK);
    
    for (k = 0; k < 12; k++)
    {
      if ((u16)ip->i_block[k] != 0){ // Read each DIRECT block if it contains data
           getblk((u16)ip->i_block[k], buf2); 
           dp = (DIR *)buf2;
           cp = buf2;
          while (cp < buf2 + 1024){
              strncpy(buf3, dp->name, dp->name_len);
              if (!strncmp(buf3, filename, dp->name_len)){
                  inode = dp->inode;
                  return inode;
              }
             cp += dp->rec_len;
             dp = (DIR *)cp;
          } 
      }
    }
}

void write_blocks(u16 inode){
    char buf1[BLK], buf2[BLK], *cp;
    u16 iblk, k, i, tmp = 0, lblk;
    
    iblk = ((inode - 1) / INODES_PER_BLOCK + 5);
    getblk((u16) iblk, buf1);
    ip = (INODE *)buf1 + (u16)((inode-1) % INODES_PER_BLOCK);
    
    setes(0x1000);
    
    for (i = 0; i<256+12; i++)
    {
        if (i < 12){
            getblk((u16)ip->i_block[i], 0);
            inces();
        }
        
        else{
            if (i == 12){
                setes(0x9000);
                getblk((u16) ip->i_block[12], buf2);
                setes(0x1000);
                for (k = 0; k < 12; k++)
                    inces();
            }
            lblk=(int)*((unsigned long *)buf2 + (i - 12));
            if (lblk == 0) return;
            getblk((u16)lblk, 0);
            inces();
        }
    }    
}

main()
{ 
    u16  i, k, iblk, iboot, ifile;
    char booter[20], tmp[20];
    
    iboot = search((u16)2, "boot");
    //prints("Enter booter name:");
    //gets(booter);
    ifile = search(iboot, "mtx");
    write_blocks(ifile);
}
