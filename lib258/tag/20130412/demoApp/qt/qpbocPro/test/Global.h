
#ifndef		_GLOBAL_H
#define		_GLOBAL_H

#ifndef		GLOBAL_DATA
#define		GLOBAL_DATA		extern
#endif

#include	<QPBOC.h>
#include	<include.h>

#ifdef	__cplusplus
extern "C"
{
#endif

GLOBAL_DATA  QTERMCONFIG	 	QConfig;
GLOBAL_DATA	 UCHAR		 	ucQTermCAPKNum;
GLOBAL_DATA	 QCAPK		 	QTermCAPK[QMAXTERMCAPKNUMS];
GLOBAL_DATA	 UCHAR		 	ucQTermAIDNum;
GLOBAL_DATA  QTERMSUPPORTAPP	QTermAID[QMAXTERMAIDNUMS];
GLOBAL_DATA	 UCHAR			ucQIPKRevokeNum;
GLOBAL_DATA	 QIPKREVOKE		QIPKRevoke[QMAXIPKREVOKENUMS];
GLOBAL_DATA	 UCHAR			ucQExceptFileNum;
GLOBAL_DATA	 QEXCEPTPAN		QExceptFile[QMAXEXCEPTFILENUMS];
GLOBAL_DATA	 QEMVCONSTPARAM		QConstParam;
GLOBAL_DATA	 QTRANSREQINFO QTransReqInfo;
#ifdef	__cplusplus
}
#endif
#endif
