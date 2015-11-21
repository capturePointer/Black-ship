#include "sailfish.h"
/**
 * While a signal handler is executing, the signal being delivered is blocked
 * Furthermore,any additional signals that were specified in the @sa_mask sign
 * set passed to sigaction when the handler was installed are also blocked.We
 * set @sa_mask to the empty set, meaning no additional signals are blocked
 * other than the signal being caugh.
 *
 */

static Sigfunc *child_signal(int signo, Sigfunc *func);


//Classic wrapper signal function
Sigfunc *
Signal(int signo, Sigfunc *func)
{
	Sigfunc		*rsig;
	rsig = signal(signo, func);
	/*check if it fails*/
	if(rsig == SIG_ERR)
		prog_error("Signal error",true,errno);

	//else return the signal
	return rsig;
}
//Custom signal cache
Sigfunc *
c_signal(int signo,Sigfunc *func)
{
	Sigfunc *rsig = NULL;

	switch(signo)
	{
		case SIGCHLD:
		{
			rsig = child_signal(SIGCHLD,func);
			break;
		}
		case SIGPIPE:
		{
			// Note: future update with the sigpipe
			break;	
		}
		default:
		{
			prog_error("Wrong signal arguments",true,errno);
			break;
		}

		//..
		//*future updates will contain more handlers
	}

	if(rsig == SIG_ERR)
		prog_error("Signal error",true,errno);

	return rsig;
}


// Private methods

static Sigfunc *
child_signal(int signo, Sigfunc *func)
{
	//node: sig => struct stigaction
	sig action,consequence;

	action.sa_handler = func;
	// set sa_mask to be emptyset
	sigemptyset(&action.sa_mask);
	// reset function if is  is interupted in the middle of the proccess
	action.sa_flags = SA_RESTART;

	if(sigaction(signo,&action,&consequence) == -1)
		return SIG_ERR;

	return consequence.sa_handler;
}
/**
 * static Sigfunc *
pipe_signal(int signo, Sigfunc *func)
{
	sig action, consequence;
	action.sa_handler =  func;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART;

		
}
*/
