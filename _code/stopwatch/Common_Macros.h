/*
 * Common_Macros.h
 *
 *  Created on: Feb 1, 2021
 *      Author: ghada
 */

#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

/*set bit no <BIT_NO> bit in register <REG> to 1*/
#define SET_BIT(REG,BIT_NO) (REG|=(1<<BIT_NO))

/*clear bit no bit <BIT_NO> in register <REG> to 0*/
#define CLEAR_BIT(REG,BIT_NO) (REG&=(~(1<<BIT_NO)))

/*toggle bit no <BIT_NO> bit in register <REG> */
#define TOGGLE_BIT(REG,BIT_NO) (REG^=(1<<BIT_NO))

/*return 1 if bit <BIT_NO> in register <REG> is 1*/
#define BIT_IS_SET (REG,BIT_NO) ((REG)&(1<<BIT_NO))

/*return 1 if bit <BIT_NO> in register <REG> is 0*/
#define BIT_IS_CLEAR (REG,BIT_NO) (!((REG)&(1<<BIT_NO)))

/*read bit no <BIT_NO> bit in register <REG> */
#define READ_BIT (REG,BIT_NO) ((REG>>BIT_NO)&(1))
#endif /* COMMON_MACROS_H_ */
