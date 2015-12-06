typedef long Align;			//假设long类型为最受限类型，按照long类型的边界对齐

union header{				//块的头部
	struct{
		union header *ptr;	//空闲块链表中的下一块
		unsigned size;		//本块的大小
	} s;
	Align x;				//强制块的对齐
};

typedef union header Header;

static Header base;			//从空链表开始
static Header *freep = NULL;//空闲链表的初始指针

//malloc函数：通用存储分配函数
void *malloc(unsigned nbytes){
	Header *p, *prevp;
	Header *morecore(unsigned);
	unsigned nunits;

	nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
	if((prevp = freep) == NULL){			//没有空闲链表
		base.s.ptr = freep = prevp = &base;
		base.s.size = 0;
	}
	for(p = prevp->s.ptr; ;prevp = p, p = p->s.ptr){
		if(p->s.size >= nunits){		//足够大
			if(p->s.size == nunits)		//正好
				prevp->s.ptr = p->s.ptr;
			else{						//分配末尾部分
				p->s.size -= nunits;
				p += p->s.size;
				p->s.size = nunits;
			}
			freep = prevp;
			return (void *)(p+1);
		}
		if(p == freep)					//闭环的空闲链表
			if((p = morecore(nunits)) == NULL)
				return NULL;	//没有剩余的存储空间
	}
}

#define NALLOC 1024		//最小申请单元，避免多次调用的开销

void free(void *);

//morecore 函数：向系统申请更多的存储空间
static Header *morecore(unsigned nu){
	char *cp, *sbrk(int);
	Header *up;

	if(nu < NALLOC)
		nu = NALLOC;
	cp = sbrk(nu * sizeof(Header));
	if( cp == (char *) -1)		//没有空间，用char*实现与具体系统无关
		return NULL;
	up = (Header *)cp;
	up->s.size = nu;
	free((void *)(up + 1));
	return freep;
}

// free 函数：将ap放入空闲块链表中
void free(void *ap){
	Header *bp, *p;

	bp = (Header *)ap - 1;		//指向块头
	//空闲链表的组织方式决定了若bp在区间内，一定不会与现有的空闲块出现覆盖的情况，空闲块是按照空闲地址的值组织的
	for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.str)	
		if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
			break;		//被释放的块在链表的开头或末尾

	if(bp + bp->s.size == p.ptr){
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	}else{
		bp->s.ptr = p->s.ptr;
	}
	if(p + p->s.size == bp){
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	}else
		p->s.ptr = bp;
	freep = p;
}
