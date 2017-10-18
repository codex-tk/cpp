#ifndef __codex_time_ctime_h__
#define __codex_time_ctime_h__

#include <codex/codex.hpp>

namespace codex {
namespace time {

class ctime {
public:
	// ����
	static bool isleapyear( const int year );
	// �ش� �⵵�� ��¥�� 
	static int  daycount( const int year );
	// �ش� �⵵ ������ ��¥�� 
	static int	daycount( const int startYear , const int endYear );
	// �ػ� �⵵�� ���� ������ ��¥��
	static int  daycount( const int year , const int month , const int day );

	static time_t convert( const tm& aTm );
	static struct tm convert( const time_t& aTm );

	/* �⺻ ���� ������� mktime �� local to utc �̹Ƿ� ������ �ٸ�..
	static void gmtime( const time_t aTime , tm& aTm );
	static void localtime( const time_t aTime , tm& aTm );
	static time_t mktime( const tm& aTm );
	
	// utc = local + bias
	// minute
	// utc - bias = local
	static int  bias();*/
};

}}

#endif
