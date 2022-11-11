#ifndef HAL_H_
#define HAL_H_

#include "../int/int.h"
#include "../pal/pal.h"
#include "../stdlib/stdlib.h"

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
	//private:
		static bool intmgr_init;
		uint32_t mstatus_last;
		void (*irq_cb[5])() = { nullptr };

		void setup_vtable();

		CLINTRegBlk* clint_reg_blk;
		PLICRegBlk* plic_reg_blk;
	};
}
#endif /* HAL_H_ */
