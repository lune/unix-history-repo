/*	conf.c	4.18	83/04/30	*/

#include "../machine/pte.h"

#include "../h/param.h"
#include "../h/inode.h"
#include "../h/fs.h"

#include "../vaxmba/mbareg.h"

#include "saio.h"

devread(io)
	register struct iob *io;
{
	int cc;

	io->i_flgs |= F_RDDATA;
	io->i_error = 0;
	cc = (*devsw[io->i_ino.i_dev].dv_strategy)(io, READ);
	io->i_flgs &= ~F_TYPEMASK;
	return (cc);
}

devwrite(io)
	register struct iob *io;
{
	int cc;

	io->i_flgs |= F_WRDATA;
	io->i_error = 0;
	cc = (*devsw[io->i_ino.i_dev].dv_strategy)(io, WRITE);
	io->i_flgs &= ~F_TYPEMASK;
	return (cc);
}

devopen(io)
	register struct iob *io;
{

	(*devsw[io->i_ino.i_dev].dv_open)(io);
}

devclose(io)
	register struct iob *io;
{

	(*devsw[io->i_ino.i_dev].dv_close)(io);
}

devioctl(io, cmd, arg)
	register struct iob *io;
	int cmd;
	caddr_t arg;
{

	return ((*devsw[io->i_ino.i_dev].dv_ioctl)(io, cmd, arg));
}

/*ARGSUSED*/
nullsys(io)
	struct iob *io;
{

	;
}

/*ARGSUSED*/
nullioctl(io, cmd, arg)
	struct iob *io;
	int cmd;
	caddr_t arg;
{

	return (ECMD);
}

int	nullsys(), nullioctl();
#if defined(VAX780) || defined(VAX750)
int	hpstrategy(), hpopen(), hpioctl();
#endif
int	upstrategy(), upopen(), upioctl();
int	rkstrategy(), rkopen(), rkioctl();
int	rastrategy(), raopen(), raioctl();
int	idcstrategy(), idcopen(), idcioctl();
#if defined(VAX780) || defined(VAX750)
int	rlstrategy(), rlopen(), rlioctl();
#endif
#ifndef BOOT
int	tmstrategy(), tmopen(), tmclose();
int	tsstrategy(), tsopen(), tsclose();
#if defined(VAX780) || defined(VAX750)
int	htstrategy(), htopen(), htclose();
int	mtstrategy(), mtopen(), mtclose();
#endif
int	utstrategy(), utopen(), utclose();
#endif

struct devsw devsw[] = {
#if defined(VAX780) || defined(VAX750)
	{ "hp",	hpstrategy,	hpopen,		nullsys,	hpioctl },
#endif
	{ "up",	upstrategy,	upopen,		nullsys,	upioctl },
	{ "hk",	rkstrategy,	rkopen,		nullsys,	rkioctl },
	{ "ra",	rastrategy,	raopen,		nullsys,	raioctl },
#if defined(VAX730)
	{ "rb",	idcstrategy,	idcopen,	nullsys,	idcioctl },
#endif
#if defined(VAX780) || defined(VAX750)
	{ "rl",	rlstrategy,	rlopen,		nullsys,	rlioctl },
#endif
#ifndef BOOT
	{ "ts",	tsstrategy,	tsopen,		tsclose,	nullioctl },
#if defined(VAX780) || defined(VAX750)
	{ "ht",	htstrategy,	htopen,		htclose,	nullioctl },
	{ "mt",	mtstrategy,	mtopen,		mtclose,	nullioctl },
#endif
	{ "tm",	tmstrategy,	tmopen,		tmclose,	nullioctl },
	{ "ut",	utstrategy,	utopen,		utclose,	nullioctl },
#endif
	{ 0, 0, 0, 0, 0 },
};
