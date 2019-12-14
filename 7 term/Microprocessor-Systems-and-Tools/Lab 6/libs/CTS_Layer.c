/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*!
 *  @file   CTS_Layer.c
 *
 *  @brief  This source file contains the API calls and one support function.     
 *
 *  @par    Project:
 *             MSP430 Capacitive Touch Library 
 *
 *  @par    Developed using:
 *             CCS Version : 5.2.1.00018, w/support for GCC extensions (--gcc)
 *  \n         IAR Version : 5.51.2 [Kickstart]
 *
 *  @version     1.1
 *                  Fixed comparison bug within the dominant element function.
 *                  Fixed convergence bugs associated with VSLOW and SLOW
 *                  tracking settings for TRADIO and TRIDIO.
 *
 *  @par    Supported API Calls:
 *              - TI_CAPT_Init_Baseline()
 *              - TI_CAPT_Update_Baseline()
 *              - TI_CAPT_Reset_Tracking()
 *              - TI_CAPT_Update_Tracking_DOI()
 *              - TI_CAPT_Update_Tracking_Rate()
 *              - TI_CAPT_Raw()
 *              - TI_CAPT_Custom()
 *              - TI_CAPT_Button()
 *              - TI_CAPT_Buttons()
 *              - TI_CAPT_Slider()
 *              - TI_CAPT_Wheel()
 */

#include "CTS_Layer.h"

/*! @defgroup GLOBAL_VARS Global Variables
 *  @{ 
 */
#ifdef TOTAL_NUMBER_OF_ELEMENTS
/*! 
 *  The baseline tracking variables 
 */
uint16_t baseCnt[TOTAL_NUMBER_OF_ELEMENTS];
#ifdef RAM_FOR_FLASH
/*! 
 *  The current measurement variables
 */
uint16_t measCnt[MAXIMUM_NUMBER_OF_ELEMENTS_PER_SENSOR];
#endif
/*! 
 *  The Status Register
 */
uint16_t ctsStatusReg = (DOI_INC+TRADOI_FAST+TRIDOI_SLOW);
#endif
/*!
 *  @}
 */

/** @defgroup CTS_API API Group
 *  @{ 
 */

/***************************************************************************//**
 * @brief   Measure the capacitance of each element within the Sensor
 * 
 *          This function selects the appropriate HAL to perform the capacitance
 *          measurement based upon the halDefinition found in the sensor 
 *          structure. 
 *          The order of the elements within the Sensor structure is arbitrary 
 *          but must be consistent between the application and configuration. 
 *          The first element in the array (counts) corresponds to the first 
 *          element within the Sensor structure.
 * @param   groupOfElements Pointer to Sensor structure to be measured
 * @param   counts Address to where the measurements are to be written
 * @return  none
 ******************************************************************************/
void TI_CAPT_Raw(const struct Sensor* groupOfElements, uint16_t * counts)
{
#ifdef RO_CSIO_TA2_WDTA
    if(groupOfElements->halDefinition == RO_CSIO_TA2_WDTA)
    {
        TI_CTS_RO_CSIO_TA2_WDTA_HAL(groupOfElements, counts);
    }
#endif

#ifdef RO_CSIO_TA2_TA3
    if(groupOfElements->halDefinition == RO_CSIO_TA2_TA3)
    {
        TI_CTS_RO_CSIO_TA2_TA3_HAL(groupOfElements, counts);
    }
#endif

#ifdef fRO_CSIO_TA2_TA3
    if(groupOfElements->halDefinition == fRO_CSIO_TA2_TA3)
    {
        TI_CTS_fRO_CSIO_TA2_TA3_HAL(groupOfElements, counts);
    }
#endif

#ifdef RO_COMPB_TB0_WDTA
    if(groupOfElements->halDefinition == RO_COMPB_TB0_WDTA)
    {
        TI_CTS_RO_COMPB_TB0_WDTA_HAL(groupOfElements, counts);
    }
#endif

#ifdef RO_PINOSC_TA0_TA1
    if(groupOfElements->halDefinition == RO_PINOSC_TA0_TA1)
    {
        TI_CTS_RO_PINOSC_TA0_TA1_HAL(groupOfElements, counts);
    }
#endif   

#ifdef fRO_PINOSC_TA0_TA1
    if(groupOfElements->halDefinition == fRO_PINOSC_TA0_TA1)
    {
        TI_CTS_fRO_PINOSC_TA0_TA1_HAL(groupOfElements, counts);
    }
#endif   
    
#ifdef RO_COMPAp_TA0_WDTp
    if(groupOfElements->halDefinition == RO_COMPAp_TA0_WDTp)
    {
        TI_CTS_RO_COMPAp_TA0_WDTp_HAL(groupOfElements, counts);
    }
#endif

#ifdef fRO_COMPAp_TA0_SW
    if(groupOfElements->halDefinition == fRO_COMPAp_TA0_SW)
    {
        TI_CTS_fRO_COMPAp_TA0_SW_HAL(groupOfElements, counts);
    }
#endif  

#ifdef fRO_COMPAp_SW_TA0
    if(groupOfElements->halDefinition == fRO_COMPAp_SW_TA0)
    {
        TI_CTS_fRO_COMPAp_SW_TA0_HAL(groupOfElements, counts);
    }
#endif  

#ifdef RO_COMPAp_TA1_WDTp
    if(groupOfElements->halDefinition == RO_COMPAp_TA1_WDTp)
    {
        TI_CTS_RO_COMPAp_TA1_WDTp_HAL(groupOfElements, counts);
    }
#endif

#ifdef fRO_COMPAp_TA1_SW
    if(groupOfElements->halDefinition == fRO_COMPAp_TA1_SW)
    {
        TI_CTS_fRO_COMPAp_TA1_SW_HAL(groupOfElements, counts);
    }
#endif  

#ifdef RC_PAIR_TA0
    if(groupOfElements->halDefinition == RC_PAIR_TA0)
    {
        TI_CTS_RC_PAIR_TA0_HAL(groupOfElements, counts);
    }
#endif

#ifdef RO_PINOSC_TA0_WDTp 
    if(groupOfElements->halDefinition == RO_PINOSC_TA0_WDTp)
    {
        TI_CTS_RO_PINOSC_TA0_WDTp_HAL(groupOfElements, counts);
    }
#endif

#ifdef RO_PINOSC_TA0 
    if(groupOfElements->halDefinition == RO_PINOSC_TA0)
    {
        TI_CTS_RO_PINOSC_TA0_HAL(groupOfElements, counts);
    }
#endif

#ifdef fRO_PINOSC_TA0_SW
    if(groupOfElements->halDefinition == fRO_PINOSC_TA0_SW)
    {
        TI_CTS_fRO_PINOSC_TA0_SW_HAL(groupOfElements, counts);
    }
#endif

#ifdef RO_COMPB_TA0_WDTA
    if(groupOfElements->halDefinition == RO_COMPB_TA0_WDTA)
    {
        TI_CTS_RO_COMPB_TA0_WDTA_HAL(groupOfElements, counts);
    }
#endif  

#ifdef fRO_COMPB_TA0_SW
    if(groupOfElements->halDefinition == fRO_COMPB_TA0_SW)
    {
        TI_CTS_fRO_COMPB_TA0_SW_HAL(groupOfElements, counts);
    }
#endif

#ifdef RO_COMPB_TA1_WDTA
    if(groupOfElements->halDefinition == RO_COMPB_TA1_WDTA)
    {
        TI_CTS_RO_COMPB_TA1_WDTA_HAL(groupOfElements, counts);
    }
#endif      

#ifdef fRO_COMPB_TA1_SW
    if(groupOfElements->halDefinition == fRO_COMPB_TA1_SW)
    {
        TI_CTS_fRO_COMPB_TA1_SW_HAL(groupOfElements, counts);
    }
#endif  

#ifdef RO_COMPB_TA1_TA0
    if(groupOfElements->halDefinition == RO_COMPB_TA1_TA0)
    {
        TI_CTS_RO_COMPB_TA1_TA0_HAL(groupOfElements, counts);
    }
#endif

#ifdef fRO_COMPB_TA1_TA0
    if(groupOfElements->halDefinition == fRO_COMPB_TA1_TA0)
    {
        TI_CTS_fRO_COMPB_TA1_TA0_HAL(groupOfElements, counts);
    }
#endif  
}

#ifdef TOTAL_NUMBER_OF_ELEMENTS
/***************************************************************************//**
 * @brief   Make a single capacitance meausrment to initialize baseline tracking
 * @param   groupOfElements Pointer to Sensor structure to be measured
 * @return  none
 ******************************************************************************/
void TI_CAPT_Init_Baseline(const struct Sensor* groupOfElements)
{
    TI_CAPT_Raw(groupOfElements, &baseCnt[groupOfElements->baseOffset]);
}

/***************************************************************************//**
 * @brief   Update baseline tracking by averaging several measurements
 * @param   groupOfElements Pointer to Sensor structure to be measured
 * @param   numberOfAverages Number of measurements to be averaged
 * @return  none
 ******************************************************************************/
void TI_CAPT_Update_Baseline(const struct Sensor* groupOfElements, uint8_t numberOfAverages)
{
	uint8_t i,j;
    #ifndef RAM_FOR_FLASH
	uint16_t *measCnt;
    measCnt = (uint16_t *)malloc(groupOfElements->numElements * sizeof(uint16_t));
    if(measCnt ==0)
    {
        while(1);
    }
    #endif
    for(j=0; j < numberOfAverages; j++)
    {
        for(i=0; i < groupOfElements->numElements; i++)
        {
            TI_CAPT_Raw(groupOfElements, measCnt);
            baseCnt[i+groupOfElements->baseOffset]
                    = measCnt[i]/2 + baseCnt[i+groupOfElements->baseOffset]/2;
        }
    }
    #ifndef RAM_FOR_FLASH
    free(measCnt);
    #endif
}

/***************************************************************************//**
 * @brief   Reset the Baseline Tracking algorithm to the default state
 * @param   none
 * @return  none
 ******************************************************************************/
void TI_CAPT_Reset_Tracking(void)
{
    ctsStatusReg = (DOI_INC+TRADOI_FAST+TRIDOI_SLOW);
}

/***************************************************************************//**
 * @brief   Update the Baseline Tracking algorithm Direction of Interest
 * @param   direction Direction of increasing or decreasing capacitance
 * @return  none
 ******************************************************************************/
void TI_CAPT_Update_Tracking_DOI(uint8_t direction)
{
	if(direction)
	{
		ctsStatusReg |= DOI_INC;
	}
	else
	{
		ctsStatusReg &= ~DOI_INC;
	}
}

/***************************************************************************//**
 * @brief   Update the baseling tracking algorithm tracking rates
 * @param   rate Rate of tracking changes in and against direction of intrest
 * @return  none
 ******************************************************************************/
void TI_CAPT_Update_Tracking_Rate(uint8_t rate)
{
    ctsStatusReg &= ~(TRIDOI_FAST+TRADOI_VSLOW); // clear fields
    ctsStatusReg |= (rate & 0xF0); // update fields
}

/***************************************************************************//**
 * @brief   Measure the change in capacitance of the Sensor
 * 
 *          This function measures the change in capacitance of each element
 *          within a sensor and updates the baseline tracking in the event that
 *          no change exceeds the detection threshold.
 *          The order of the elements within the Sensor structure is arbitrary 
 *          but must be consistent between the application and configuration. 
 *          The first element in the array (deltaCnt) corresponds to the first 
 *          element within the Sensor structure.
 * @param   groupOfElements Pointer to Sensor structure to be measured
 * @param   deltaCnt Address to where the measurements are to be written
 * @return  none
 ******************************************************************************/
void TI_CAPT_Custom(const struct Sensor* groupOfElements, uint16_t * deltaCnt)
{ 
    uint8_t j;
    uint16_t tempCnt, remainder;
    ctsStatusReg &= ~ EVNT;
	
    TI_CAPT_Raw(groupOfElements, &deltaCnt[0]); // measure group of sensors
          
    for (j = 0; j < (groupOfElements->numElements); j++)
    {  
        tempCnt = deltaCnt[j];
        if(deltaCnt[j])
        {
        if(((ctsStatusReg & DOI_MASK)
        		&& (groupOfElements->halDefinition & RO_MASK))
            ||
         ((!(ctsStatusReg & DOI_MASK))
        		&& (!(groupOfElements->halDefinition & RO_MASK))))
        { 
        	/*
        	 * Interested in a decrease in counts. Either the decrease
        	 * represents an increase in capacitance (a touch) with the
        	 * RO method, or the decrease represents a decrease in capacitance
        	 * (release) with the fRO or RC methods.
        	 */
            if(baseCnt[j+groupOfElements->baseOffset] < deltaCnt[j])            
            {
                /*
                 * The measured value is greater than the baseline therefore
                 * no detection logic is needed.  The measured value is
                 * preserved in tempCnt and is used for baseline updates.
                 */
                deltaCnt[j] = 0;
                if(((groupOfElements->arrayPtr[j])->threshold)
                &&
                (baseCnt[j+groupOfElements->baseOffset]
                    +((groupOfElements->arrayPtr[j])->threshold/2) < tempCnt))
                {
                	/*
                	 * When the threshold is valid (non-calibration state),
                	 * limit the measurement to the baseline + threshold/2.
                	 */
                    tempCnt = baseCnt[j+groupOfElements->baseOffset]
                                +((groupOfElements->arrayPtr[j])->threshold)/2;
                }
            }
            else
            {
                /*
                 * deltaCnt now represents the magnitude of change relative to
                 * the baseline.
                 */
                deltaCnt[j] = baseCnt[j+groupOfElements->baseOffset]
                                  - deltaCnt[j];
            }
        }
        if(((!(ctsStatusReg & DOI_MASK))
        		&& (groupOfElements->halDefinition & RO_MASK))
           ||
             ((ctsStatusReg & DOI_MASK)
            	&& (!(groupOfElements->halDefinition & RO_MASK))))
        { 
        	/*
        	 * Interested in an increase in counts. Either the increase
        	 * represents a decrease in capacitance (a release) with the
        	 * RO method, or the increase represents a increase in capacitance
        	 * (touch) with the fRO or RC methods.
        	 */
            if(baseCnt[j+groupOfElements->baseOffset] > deltaCnt[j])            
            {
                /*
                 * The measured value is less than the baseline therefore
                 * no detection logic is needed.  The measured value is
                 * preserved in tempCnt and is used for baseline updates.
                 */
                deltaCnt[j] = 0;
                if(((groupOfElements->arrayPtr[j])->threshold)
                &&
                (baseCnt[j+groupOfElements->baseOffset]
                     -((groupOfElements->arrayPtr[j])->threshold/2) > tempCnt))
                {
                	/*
                	 * When the threshold is valid (non-calibration state),
                	 * limit the measurement to the baseline - threshold/2.
                	 */
                    tempCnt = baseCnt[j+groupOfElements->baseOffset]
                                -((groupOfElements->arrayPtr[j])->threshold)/2;
                }
            }
            else       
            {
                /*
                 * deltaCnt now represents the magnitude of change relative to
                 * the baseline.
                 */
                deltaCnt[j] = deltaCnt[j]
                              - baseCnt[j+groupOfElements->baseOffset];
            }         
        }
            
        // This section updates the baseline capacitance************************
        if (deltaCnt[j]==0)
        { // if delta counts is 0, then the change in capacitance was opposite
          // the direction of interest.  The baseCnt[i] is updated with the
          // saved tempCnt value for the current index value 'i'.
        	remainder = 0;
			switch ((ctsStatusReg & TRADOI_VSLOW))
			{
			case TRADOI_FAST://Fast
					tempCnt = tempCnt/2;
					baseCnt[j+groupOfElements->baseOffset]
					= (baseCnt[j+groupOfElements->baseOffset]/2);
					break;
			case TRADOI_MED://Medium
					tempCnt = tempCnt/4;
					baseCnt[j+groupOfElements->baseOffset]
					= 3*(baseCnt[j+groupOfElements->baseOffset]/4);
					break;
			case TRADOI_SLOW://slow
				  /* Calculate remainder associated with (x + 63*y)/64 */
				  remainder = 0x003F & baseCnt[j+groupOfElements->baseOffset];
				  remainder = remainder * 63;
				  remainder += 0x003F & tempCnt;           
				  remainder = remainder >> 6;
				  tempCnt = tempCnt/64;
				  baseCnt[j+groupOfElements->baseOffset]
				  = 63*(baseCnt[j+groupOfElements->baseOffset]/64);
					break;
			case TRADOI_VSLOW://very slow
                /* Calculate remainder associated with (x+127*y)/128 */
                remainder = 0x007F & baseCnt[j+groupOfElements->baseOffset];
                remainder = remainder * 127;
                remainder += 0x007F & tempCnt;
                remainder = remainder >> 7;
				tempCnt = tempCnt/128;
				baseCnt[j+groupOfElements->baseOffset]
				= 127*(baseCnt[j+groupOfElements->baseOffset]/128);
			break;
			}
            /* Base_Capacitance = (Measured_Capacitance/Z)
		                        + Y*(Base_Capacitance/Z) */
		    tempCnt += remainder;
            baseCnt[j+groupOfElements->baseOffset] += tempCnt;
            /* In the case that DOI is set and */
            if(groupOfElements->halDefinition & RO_MASK)
            {
			    /*  If the RO_MASK is set then the direction of interest is
			     *  decreasing (counts decrease with capacitance) and therefore
			     *  movement against the direction of interest would be an
			     *  increase: increment.
			     */
                baseCnt[j+groupOfElements->baseOffset]++;
            }
            else
            {
			    /*  RO_MASK is not set and therefore a decrease is against
			     *  the direction of interest: decrement
			     */
                baseCnt[j+groupOfElements->baseOffset]--;
            }
        }
        /* deltaCnt is either 0, less than threshold, or greater than 
		   threshold, never negative. */
        else if(deltaCnt[j]<(groupOfElements->arrayPtr[j])->threshold 
		         && !(ctsStatusReg & PAST_EVNT))
        {    //if delta counts is positive but less than threshold,
        	remainder = 1;
            switch ((ctsStatusReg & TRIDOI_FAST))
            {
            case TRIDOI_VSLOW:
                tempCnt = 0;
				break;
			case TRIDOI_SLOW://slow
				remainder = 2;
			    tempCnt = 0;
			    break;
			case TRIDOI_MED://medium
				tempCnt = tempCnt/4;
				baseCnt[j+groupOfElements->baseOffset] = 3*(baseCnt[j+groupOfElements->baseOffset]/4);
				break;
			case TRIDOI_FAST://fast
				tempCnt = tempCnt/2;
				baseCnt[j+groupOfElements->baseOffset] = (baseCnt[j+groupOfElements->baseOffset]/2);
				break;
			}
          /*
           *  Base_Capacitance = (Measured_Capacitance/Z) +
           *  Y*(Base_Capacitance/Z)
           */
          baseCnt[j+groupOfElements->baseOffset] += tempCnt;
          if(groupOfElements->halDefinition & RO_MASK)
          {
			    /*  If the RO_MASK is set then the direction of interest is
			     *  decreasing (counts decrease with capacitance) and
			     *  therefore movement in the direction of interest would
			     *  be a decrease: decrement.
			     */
              baseCnt[j+groupOfElements->baseOffset]-= remainder;
          }
          else
          {
			    /*  RO_MASK is not set and therefore an increase is in the
			     *  direction of interest: increment
			     */
              baseCnt[j+groupOfElements->baseOffset]+= remainder;
          }
        }
        //if delta counts above the threshold, event has occurred
        else if(deltaCnt[j]>=(groupOfElements->arrayPtr[j])->threshold)
        {
          ctsStatusReg |= EVNT;
          ctsStatusReg |= PAST_EVNT;
        }
        }
    }// end of for-loop
    if(!(ctsStatusReg & EVNT))
    {
      ctsStatusReg &= ~PAST_EVNT;
    }
}

/***************************************************************************//**
 * @brief   Determine if a button is being pressed
 * @param   groupOfElements Pointer to button to be scanned
 * @return  result Indication if button is (1) or is not (0) being pressed
 ******************************************************************************/
uint8_t TI_CAPT_Button(const struct Sensor * groupOfElements)
{
    uint8_t result = 0;
    
    #ifndef RAM_FOR_FLASH
    uint16_t *measCnt;
    measCnt = (uint16_t *)malloc(groupOfElements->numElements * sizeof(uint16_t));
    if(measCnt ==0)
    {
        while(1);
    }
    #endif
    
    TI_CAPT_Custom(groupOfElements, measCnt);
    #ifndef RAM_FOR_FLASH
    free(measCnt);
    #endif
    if(ctsStatusReg & EVNT)
    {
        result = 1;
    }    
    return result;
}

/***************************************************************************//**
 * @brief   Determine which button if any is being pressed
 * @param   groupOfElements Pointer to buttons to be scanned
 * @return  result pointer to element (button) being pressed or 0 none
 ******************************************************************************/
const struct Element *TI_CAPT_Buttons(const struct Sensor *groupOfElements)
{
    uint8_t index;
    #ifndef RAM_FOR_FLASH
    uint16_t *measCnt;
    measCnt = (uint16_t *)malloc(groupOfElements->numElements * sizeof(uint16_t));
    if(measCnt ==0)
    {
        while(1);
    }
    #endif
    TI_CAPT_Custom(groupOfElements, measCnt);
    
    if(ctsStatusReg & EVNT)
    {
        index = Dominant_Element(groupOfElements, measCnt);
        //ctsStatusReg &= ~EVNT;
        index++;
    }
    else 
    {
        index = 0;
    }
    #ifndef RAM_FOR_FLASH
    free(measCnt);
    #endif
    if(index)
    {
      return groupOfElements->arrayPtr[index-1];
    }
    return 0;
}

#ifdef SLIDER
/***************************************************************************//**
 * @brief   Determine the position on a slider
 * @param   groupOfElements Pointer to slider
 * @return  result position on slider or illegal value if no touch
 ******************************************************************************/
uint16_t TI_CAPT_Slider(const struct Sensor* groupOfElements)
{
    uint8_t index;
    int16_t position;
    // allocate memory for measurement
    #ifndef RAM_FOR_FLASH
    uint16_t *measCnt;
    measCnt = (uint16_t *)malloc(groupOfElements->numElements * sizeof(uint16_t));
    if(measCnt ==0)
    {
        while(1);
    }
    #endif
    position = ILLEGAL_SLIDER_WHEEL_POSITION;
    //make measurement
    TI_CAPT_Custom(groupOfElements, measCnt);
    
    // Use EVNT flag to determine if slider was touched.
    // The EVNT flag is a global variable and managed within the TI_CAPT_Custom
    // function.
    if(ctsStatusReg & EVNT)
    {
        index = Dominant_Element(groupOfElements, &measCnt[0]);
        // The index represents the element within the array with the highest
        // return.
        if(index == 0)
        {
          // Special case of 1st element in slider, add 1st, last, and 2nd
          position = measCnt[0] + measCnt[1];
        }
        else if(index == (groupOfElements->numElements -1))
        {
          // Special case of Last element in slider, add last,
          // 1st, and 2nd to last
          position = measCnt[groupOfElements->numElements -1]
                   + measCnt[groupOfElements->numElements -2];
        }
        else
        {
            position = measCnt[index] + measCnt[index+1] + measCnt[index-1];
        } 
        // Determine if sensor threshold criteria is met
        if(position >= groupOfElements->sensorThreshold)
        {
            // calculate position
    	    position = index
    	    		       *(groupOfElements->points
    	    		           /groupOfElements->numElements);
            position += (groupOfElements->points/groupOfElements->numElements)
            		        /2;
            if(index == 0)
            {
              // Special case of 1st element in slider, which only has one 
              // neighbor, measCnt[1]. measCnt is limited to maxResponse 
              // within dominantElement function
              if(measCnt[1])
              {
                  position += (measCnt[1]
                                  *(groupOfElements->points
                                		/groupOfElements->numElements))/100;
              }
              else
              {
            	  /* Calculate position based upon measCnt[0] */
                  position = (measCnt[0]
                                  *(groupOfElements->points
                		                /groupOfElements->numElements)/2)/100;
              }
            }
            else if(index == (groupOfElements->numElements -1))
            {
              // Special case of Last element in slider, which only has one 
              // neighbor, measCnt[x-1] or measCnt[numElements-1]
              if(measCnt[index-1])
              {
                  position -= (measCnt[index-1]
                                   *(groupOfElements->points
                                		 /groupOfElements->numElements))/100;
              }
              else
              {
                  position = groupOfElements->points;
                  position -= (measCnt[index]
                                  *(groupOfElements->points
                                		/groupOfElements->numElements)/2)/100;
              }
            }
            else
            {
                  position += (measCnt[index+1]*(groupOfElements->points/groupOfElements->numElements))/100;
                  position -= (measCnt[index-1]*(groupOfElements->points/groupOfElements->numElements))/100;
            }  
            if((position > groupOfElements->points) || (position < 0))
            {
                  position = ILLEGAL_SLIDER_WHEEL_POSITION;
            }     
        }
        else
        {
            position = ILLEGAL_SLIDER_WHEEL_POSITION;
        }
    }
    #ifndef RAM_FOR_FLASH
    free(measCnt);
    #endif
    return position;
}
#endif

#ifdef WHEEL
/***************************************************************************//**
 * @brief   Determine the position on a wheel
 * @param   groupOfElements Pointer to wheel
 * @return  result position on wheel or illegal value if no touch
 ******************************************************************************/
uint16_t TI_CAPT_Wheel(const struct Sensor* groupOfElements)
{
    uint8_t index;
    int16_t position;
    // allocate memory for measurement
    #ifndef RAM_FOR_FLASH
    uint16_t *measCnt;
    measCnt = (uint16_t *)malloc(groupOfElements->numElements * sizeof(uint16_t));
    if(measCnt ==0)
    {
        while(1);
    }
    #endif
    position = ILLEGAL_SLIDER_WHEEL_POSITION;
    //make measurement
    TI_CAPT_Custom(groupOfElements, measCnt);
    // Translate the EVNT flag from an element level EVNT to a sensor level EVNT.
    // The sensor must read at least 75% cumulative response before indicating a 
    // touch.
    if(ctsStatusReg & EVNT)
    {
        index = Dominant_Element(groupOfElements, &measCnt[0]);
        // The index represents the element within the array with the highest return.
        // 
        if(index == 0)
        {
          // Special case of 1st element in slider, add 1st, last, and 2nd
          position = measCnt[0] + measCnt[groupOfElements->numElements -1] + measCnt[1];
        }
        else if(index == (groupOfElements->numElements -1))
        {
          // Special case of Last element in slider, add last, 1st, and 2nd to last
          position = measCnt[index] + measCnt[0] + measCnt[index-1];
        }
        else
        {
          position = measCnt[index] + measCnt[index+1] + measCnt[index-1];
        } 
        if(position > groupOfElements->sensorThreshold)
        {
            //index = Dominant_Element(groupOfElements, &measCnt[0]);
            // The index represents the element within the array with the highest return.
            // 
            position = index*(groupOfElements->points/groupOfElements->numElements);
            position += (groupOfElements->points/groupOfElements->numElements)/2;
            if(index == 0)
            {
              // Special case of 1st element in slider, which only has one neighbor, measCnt[1]
              // measCnt is limited to maxResponse within dominantElement function
              position += (measCnt[1]*(groupOfElements->points/groupOfElements->numElements))/100;
              position -= (measCnt[groupOfElements->numElements -1]*(groupOfElements->points/groupOfElements->numElements))/100;
              if(position < 0)
              {
                position = position + (int16_t)groupOfElements->points;
                position++;  // -1 goes to points
              }
            }
            else if(index == (groupOfElements->numElements -1))
            {
              // Special case of Last element in slider, which only has one neighbor, measCnt[x-1] or measCnt[numElements-1]
              // measCnt is limited to maxResponse within dominantElement function
              position += (measCnt[0]*(groupOfElements->points/groupOfElements->numElements))/100;
              position -= (measCnt[index-1]*(groupOfElements->points/groupOfElements->numElements))/100;
              if(position > groupOfElements->points)
              {
                position = position - (int16_t)groupOfElements->points;
                position--; // points+1 goes to 0
              }
            }
            else
            {
              position += (measCnt[index+1]*(groupOfElements->points/groupOfElements->numElements))/100;
              position -= (measCnt[index-1]*(groupOfElements->points/groupOfElements->numElements))/100;
            } 
            if((position > groupOfElements->points) || position < 0)
            {
                  position = ILLEGAL_SLIDER_WHEEL_POSITION;
            }     
        }
        else 
        {
            position = ILLEGAL_SLIDER_WHEEL_POSITION;
        }
    }
    #ifndef RAM_FOR_FLASH
    free(measCnt);
    #endif
    return position;
}
#endif

/** @} */

/***************************************************************************//**
 * @defgroup CTS_support Support Group
 * @ingroup CTS_API
 ******************************************************************************/
  
/***************************************************************************//**
 * @ingroup CTS_support
 * @brief   Determine which element within a sensor has the largest response
 * 
 *          This function compares and normalizes the change in capacitance to
 *          determine which element has the dominant response.  The deltaCnt
 *          values for each element that exceed the threshold are also 
 *          converted from a 'raw' measurement to a percentage of the maximum 
 *          response.
 * @param   groupOfElements Pointer to buttons to be scanned
 * @param   deltaCnt Address to where the measurements are to be written
 * @return  result index to the element which is dominant
 ******************************************************************************/
uint8_t Dominant_Element(const struct Sensor* groupOfElements, uint16_t* deltaCnt)
{
    uint8_t i;
    uint16_t percentDelta=0; 
    uint8_t dominantElement=0;
    for(i=0;i<groupOfElements->numElements;i++)
    {  
        if(deltaCnt[i]>=(groupOfElements->arrayPtr[i])->threshold)
        {
            if(deltaCnt[i] > ((groupOfElements->arrayPtr[i])->maxResponse))
            {
                deltaCnt[i] = (groupOfElements->arrayPtr[i])->maxResponse;
                // limit response to the maximum
            }
            // (maxResponse - threshold) cannot exceed 655
            // 100*(delta - threshold) / (maxResponse - threshold)
            deltaCnt[i] = (100*(deltaCnt[i]
                           -(groupOfElements->arrayPtr[i])->threshold))
            		         /((groupOfElements->arrayPtr[i])->maxResponse
            				    -(groupOfElements->arrayPtr[i])->threshold);
            if(deltaCnt[i] >= percentDelta)
            {
                //update percentDelta
                percentDelta = deltaCnt[i];
                dominantElement = i;
            }
        }
        else
        {
            deltaCnt[i] = 0;
        }
    } // end for loop
    return dominantElement;
}
#endif
