#include "apue.h"
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define	ARRAY_SIZE	40000
#define	MALLOC_SIZE	100000
#define	SHM_SIZE	100000
#define	SHM_MODE	0600	/* user read/write */

char	array[ARRAY_SIZE];	/* uninitialized data = bss */

int
main(void)
{
	{
		// svipc(7)
		struct ipc_perm ipc_perm_ = {};
		ipc_perm_.mode = 0666;
		ipc_perm_.mode = 0000;
		(void)(IPC_CREAT | IPC_EXCL | IPC_NOWAIT);     // mode bits
		(void)IPC_PRIVATE;                             // special key
		(void)IPC_RMID, (void)IPC_SET, (void)IPC_STAT; // commands
		(void)IPC_INFO; // ipcs(1) uses this?
	}

	// 15.7 Message Queues
	// TODO: POSIX message queue (mq_overview(7))
	{
		int rv;
		key_t key;

		// /proc/sys/kernel/msgmax 8192
		// /proc/sys/kernel/msgmnb 16384
		// /proc/sys/kernel/msgmni 32000
		// /proc/sys/kernel/msg_next_id -1

		// show: ipcs -q
		// remove: ipcrm -Q 0x71060006

		key = ftok("/dev/null", 'q'); // 1896218630 (0x71060006)

		rv = msgget(key, 0600); // -1 if not exist; 98306 if exists
		rv = msgget(key, 0600 | IPC_CREAT);            // 98306
		rv = msgget(key, 0600 | IPC_CREAT | IPC_EXCL); // -1; exists

		// IPC_PRIVATE means IPC_NEW
		// maybe same as `ipcmk -Q`
		// rv = msgget(IPC_PRIVATE, 0);

		int id = msgget(key, 0600); // 98306

		// TODO: msgsnd msgrcv
		// struct msgbuf msgp = {};
		// rv = msgsnd(id, &msgp, 0, 0);

		(void)IPC_RMID, (void)IPC_SET, (void)IPC_STAT, (void)IPC_INFO;
		(void)MSG_STAT, (void)MSG_INFO, (void)MSG_STAT_ANY;

		struct msqid_ds buf = {};
		rv = msgctl(id, IPC_STAT, &buf); // 0

		struct msginfo buf2 = {};
		rv = msgctl(id, IPC_INFO, (struct msqid_ds *)&buf2); // 2

		rv = msgctl(id, IPC_RMID, NULL); // 0
		rv = 0;
	}

	// 15.8 Semaphores
	//
	// TODO: POSIX semafore (sem_overview(7))
	//
	// /proc/sys/kernel/sem 32000   1024000000 500  32000
	//                      SEMMSL  SEMMNS     ?    SEMMNI
	{
		int id;
		int rv;
		key_t key;

		key = ftok("/dev/null", 's'); // 1929773062

		// aborted

		id = semget(key, 9, 0600 | IPC_CREAT);

		struct sembuf sops = {};
		sops.sem_num = 0;
		sops.sem_op = 0;
		semop(id, &sops, 0);

		// rv = semctl(id, )

		struct semid_ds semid_ds_ = {};
		semid_ds_.sem_perm.mode = 0x000;
	}

	// 15.9 shmem
	// TODO: POSIX shared memory (shm_overview(7))
	{
		int rv;
		key_t key;

		// TODO

		key = ftok("/dev/null", 'm');
		struct shmid_ds shmid_ds_ = {};

		shmid_ds_.shm_perm.mode = 0x000;
	}

	int		shmid;
	char	*ptr, *shmptr;

	printf("array[] from %p to %p\n", (void *)&array[0],
	  (void *)&array[ARRAY_SIZE]);
	printf("stack around %p\n", (void *)&shmid);

	if ((ptr = malloc(MALLOC_SIZE)) == NULL)
		err_sys("malloc error");
	printf("malloced from %p to %p\n", (void *)ptr,
	  (void *)ptr+MALLOC_SIZE);

	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0)
		err_sys("shmget error");
	// if ((shmptr = shmat(shmid, 0, 0)) == (void *)-1)
	if ((shmptr = shmat(shmid, NULL, 0)) == (void *)-1)
		err_sys("shmat error");
	printf("shared memory attached from %p to %p\n", (void *)shmptr,
	  (void *)shmptr+SHM_SIZE);

	if (shmctl(shmid, IPC_RMID, 0) < 0)
		err_sys("shmctl error");

	// sys/shm.h
	// ipcs(1) to show info
	// ipcs -h
	// ipcs --human
	// aborted
	{
		int rv, e;
		struct shmid_ds buf = {};
		struct shminfo buf2 = {};
		struct shm_info buf3 = {};

		(void)IPC_PRIVATE, (void)(key_t)1, (void)(key_t)2; // key
		(void)(IPC_CREAT | IPC_EXCL | SHM_HUGETLB | SHM_NORESERVE |
		       0666); // shmflg

		rv = shmctl((key_t)9999, IPC_RMID, NULL);          // can't remove?
		e = errno;
		shmid = shmget((key_t)9999, 99, 0777);             // first: -1, second: 37322768
		shmid = shmget((key_t)9999, 99, IPC_CREAT | 0777); // 37322768
		shmid = shmget((key_t)9999, 99, IPC_CREAT | 0777); // same
		shmid = shmget((key_t)9999, 0, 0777);              // same

		rv = shmctl(shmid, IPC_STAT, &buf);
		rv = shmctl(shmid, IPC_INFO, (struct shmid_ds *)&buf2);
		rv = shmctl(shmid, SHM_INFO, (struct shmid_ds *)&buf3);
		(void)SHM_STAT;
		(void)SHM_LOCK;
		(void)SHM_UNLOCK;

		shmptr = shmat(shmid, NULL, 0);
		shmptr = shmat(shmid, (void *)0x7fff7fff0000, SHM_RND);

		rv = shmctl(shmid, IPC_RMID, NULL);

		shmid = 0;
	}

	exit(0);
}
