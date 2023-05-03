#ifndef HAL_H_
#define HAL_H_

#include "int.h"
#include "pal.h"
#include "stdlib.h"

namespace HAL {
	namespace {
		uint32_t err_flg;
	}

	uint32_t get_err();
	void hal_init();

	class GPIO {
	public:
		GPIO();
		static GPIO* open(GPIOPort port);
		GPIO(GPIO const&)=delete;
		void operator=(GPIO const&)=delete;

		void enable_input(GPIOPin pin);
		uint32_t read_input(GPIOPin pin);
		void enable_output(GPIOPin pin);
		void set_output(GPIOPin pin, LogicLevel lvl);
	private:
		static bool gpioa_init;
		GPIORegBlk* reg_blk;
	};

	class IntMgr {
	public:
		IntMgr();
		static IntMgr* get();
		IntMgr(IntMgr const&)=delete;
		void operator=(IntMgr const&)=delete;

		void trig_sw_int();
		void clr_sw_int();
		void entr_critsec();
		void exit_critsec();
		void reg_int_cb(void (*cb)(), IRQMap irq);

		// MTIME and MTIMECMP read/write commands
		uint32_t read_mtime();
		uint32_t read_mtimecmp();
		void write_mtimecmp(uint32_t);
		void increment_mtimecmp(uint32_t);
		void increment_mtimecmp_past_mtime(uint32_t);

		// ENABLE and DISABLE timer, software, and external interrupts
		void enable_tm_interrupts();
		void enable_sw_interrupts(); 
		void enable_ext_interrupts();
		void disable_tm_interrupts();
		void disable_sw_interrupts(); 
		void disable_ext_interrupts();

		// SET and GET user callback handlers for interrupts
		void set_timerISR(void (*)(uint32_t), uint32_t);
		void set_swISR(void (*)());
		void set_extISR(void (*)(uint32_t), uint32_t);
		void (*get_timerISR(uint32_t *))(uint32_t);
		void (*get_swISR())();
		void (*get_extISR(uint32_t))(uint32_t);

		// EXTERNAL interrupts functions
      void enable_extISR(uint32_t);
		void disable_extISR(uint32_t);
		void set_priority_extISR(uint32_t, uint32_t);
		void set_threshold_extISR(uint32_t);

		// CLAIM and COMPLETE external interrupts
		uint32_t claim_extISR();
		void complete_extISR(uint32_t);

	private:
		static bool intmgr_init;
		uint32_t mstatus_last;

		void (*irq_cb[5])() = { nullptr };
		void setup_vtable();

		CLINTRegBlk* clint_reg_blk;
		PLICRegBlk* plic_reg_blk; 

		// User callback ISR handlers
		uint32_t user_mtimecmp_interval = 0;
      void (*user_timerISR)(uint32_t) = NULL;
		void (*user_swISR)() = NULL;
      void (*user_extISR[N_INTS])(uint32_t);
	};
	
	class PWM {  
	public:
		PWM();
		static PWM* setupChannels(); // function to set up channels
		PWM(PWM const&)=delete;
		void operator=(PWM const&)=delete;

		///test func with direct access to reg.
		static PWM* setup();  
		void setP(uint32_t value);
		void enableChanl();

		//************************************************************
		class Channel {
		public:
			Channel();

			void setPeriod(uint32_t value);
			uint32_t  getPeriod();
			void setDuty(float percentage);
			uint32_t  getDuty();
			void setControlRegister(uint8_t enable, uint8_t polarity, uint8_t alignment);

		private:
			friend class PWM; // declare PWM as a friend class to allow access to private members
			__IO uint32_t* per;
			__IO uint32_t* duty;
			__IO uint8_t* ctrl;
			uint32_t curr_per;
			uint32_t curr_duty;
		};
		int num_of_channle = 2;
		Channel channels[2];  //2 -> num of channel
		
		//PWMRegBlk* pwm_reg_blk; //move it public to check reg val for testing *****
	private:
		static bool pwm_init;
		PWMRegBlk* pwm_reg_blk;
		
	};
}
#endif /* HAL_H_ */
