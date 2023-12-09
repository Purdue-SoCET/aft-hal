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
		void set_direct(GPIOPin pin, LogicLevel lvl);
		void set_intrpt(GPIOPin pin, LogicLevel lvl);
		void set_pos_edge(GPIOPin pin, LogicLevel lvl);
		void set_neg_edge(GPIOPin pin, LogicLevel lvl);
		void clear_intrpt(GPIOPin pin);
		bool read_intrpt_status(GPIOPin pin);
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
      void enable_extISR(uint8_t select,uint32_t);
		void disable_extISR(uint8_t select,uint32_t);
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
		PWM(PWM const&)=delete;
		void operator=(PWM const&)=delete;
		static PWM* setup();  
		void setPeriod(int channel_num,uint32_t value);
		uint32_t  getPeriod(int channel_num);
		void setDuty(int channel_num,uint32_t value);
		uint32_t  getDuty(int channel_num);
		void setControlRegister(int channel_num,uint8_t enable, uint8_t polarity, uint8_t alignment);

		
	private:
		static bool pwm_init;
		PWMRegBlk* pwm_reg_blk;
		
	};

	class TMR {
	public:
		typedef struct {
            bool enable : 1;
            bool edge : 1;
            uint8_t outputMode : 2;
            uint8_t inputSource : 2;
            uint8_t inputMode : 2;
            bool interruptEnable : 1;
        } TimerChannelControlReg;  // for tccmr
		TMR();
		TMR(TMR const&)=delete;
		static TMR* get_tmr();  //create and return a static tmr address
		void tmr_enable(); //tcr enable
		void tmr_disable();//tcr disable
		void tmr_IRQenable(); //tcr inturrupt enable
		void tmr_IRQdisable();//tcr inturrupt disable
		uint32_t get_tmr_count();//tcnt, read timer count 
		void tmr_config(uint32_t prescaler, uint32_t reloadval); //tpsc & tarr, setup prescaler and reload value
		void channel_config(uint32_t channel, TimerChannelControlReg controlReg);//tcmmr, configure channel enable, mode
		void set_tccr(uint32_t channel, uint32_t tccr_val);// set tccr value for a channel

	private:
		static bool tmr_init;
		TimerRegBlk* tmr_reg_blk;
	};

	class IOMux {
	public:
		IOMux();
		IOMux(IOMux const&)=delete;
		static IOMux* IOMux_init();
		void set_config(int pin, uint32_t func_sel);
		void clear_config(int pin, uint32_t func_sel);
	private:
		static bool iomux_init;
		IOMuxRegBlk* iomux_reg_blk;
	};

	class DMA{
	public:
		typedef struct {
			bool TCIE;
			bool HTIE;
			bool SRC;
			bool DST;
			uint8_t PSIZE;
			bool trig_en;
			bool circular;
			bool IDST;
			bool ISRC;
		} dmaControlConfig;
		DMA();
		DMA(DMA const&)=delete;
		static DMA* dma_setup();
		void dma_config(dmaControlConfig config);
		void set_source(uint32_t src_addr);
		void set_dest(uint32_t dest_addr);
		void set_size(uint32_t bytes);
		void startTxf();
		void stopTxf();
		bool checkeErr();
	private:
		static bool dma_init;
		DMARegBlk* dma_reg_blk;
	};
//state reg & 0x2, error
//state uart_regs->txstate & 1, ready to send
//uart_regs->txdata = byte | (1u << 24);, send byte
//state reg << 16, BAUD_CYCLES
//uint8_t count = (uart_regs->rxdata; >> 24) & 0xFF;
	class UART {
	public:
		UART();
		UART(UART const&)=delete;
		static UART* UART_init();
		void set_Rx_BR(uint32_t BAUD_CYCLES);
		void set_Tx_BR(uint32_t BAUD_CYCLES);
		void send_byte(uint8_t byte);
		uint32_t get_RX_data();
		uint8_t get_RX_NumByteRead();
		bool checkRx_state();
		bool checkTx_state();
		uint8_t get_TX_BR();
	private:
		static bool uart_init;
		UARTRegBlk* uart_reg_blk;
	};


}
#endif /* HAL_H_ */
