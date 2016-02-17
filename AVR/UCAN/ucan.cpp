#ifndef UCAN_h
	#include <ucan.h>
#endif

//MCP Interface code from MCP_CAN
#define spi_readwrite SPI.transfer
#define spi_read() spi_readwrite(0x00)

/*********************************************************************************************************
** Function name:           mcp2515_reset
** Descriptions:            reset the device
*********************************************************************************************************/
void MCP_CAN::mcp2515_reset(void)                                      
{
    MCP2515_SELECT();
    spi_readwrite(MCP_RESET);
    MCP2515_UNSELECT();
    delay(10);
}

/*********************************************************************************************************
** Function name:           mcp2515_readRegister
** Descriptions:            read register
*********************************************************************************************************/
INT8U MCP_CAN::mcp2515_readRegister(const INT8U address)                                                                     
{
    INT8U ret;

    MCP2515_SELECT();
    spi_readwrite(MCP_READ);
    spi_readwrite(address);
    ret = spi_read();
    MCP2515_UNSELECT();

    return ret;
}

/*********************************************************************************************************
** Function name:           mcp2515_readRegisterS
** Descriptions:            read registerS
*********************************************************************************************************/
void MCP_CAN::mcp2515_readRegisterS(const INT8U address, INT8U values[], const INT8U n)
{
	INT8U i;
	MCP2515_SELECT();
	spi_readwrite(MCP_READ);
	spi_readwrite(address);
	// mcp2515 has auto-increment of address-pointer
	for (i=0; i<n; i++) 
    {
		values[i] = spi_read();
	}
	MCP2515_UNSELECT();
}

/*********************************************************************************************************
** Function name:           mcp2515_setRegister
** Descriptions:            set register
*********************************************************************************************************/
void MCP_CAN::mcp2515_setRegister(const INT8U address, const INT8U value)
{
    MCP2515_SELECT();
    spi_readwrite(MCP_WRITE);
    spi_readwrite(address);
    spi_readwrite(value);
    MCP2515_UNSELECT();
}

/*********************************************************************************************************
** Function name:           mcp2515_setRegisterS
** Descriptions:            set registerS
*********************************************************************************************************/
void MCP_CAN::mcp2515_setRegisterS(const INT8U address, const INT8U values[], const INT8U n)
{
    INT8U i;
    MCP2515_SELECT();
    spi_readwrite(MCP_WRITE);
    spi_readwrite(address);
       
    for (i=0; i<n; i++) 
    {
        spi_readwrite(values[i]);
    }
    MCP2515_UNSELECT();
}

/*********************************************************************************************************
** Function name:           mcp2515_modifyRegister
** Descriptions:            set bit of one register
*********************************************************************************************************/
void MCP_CAN::mcp2515_modifyRegister(const INT8U address, const INT8U mask, const INT8U data)
{
    MCP2515_SELECT();
    spi_readwrite(MCP_BITMOD);
    spi_readwrite(address);
    spi_readwrite(mask);
    spi_readwrite(data);
    MCP2515_UNSELECT();
}

/*********************************************************************************************************
** Function name:           mcp2515_readStatus
** Descriptions:            read mcp2515's Status
*********************************************************************************************************/
INT8U MCP_CAN::mcp2515_readStatus(void)                             
{
	INT8U i;
	MCP2515_SELECT();
	spi_readwrite(MCP_READ_STATUS);
	i = spi_read();
	MCP2515_UNSELECT();
	
	return i;
}

/*********************************************************************************************************
** Function name:           mcp2515_setCANCTRL_Mode
** Descriptions:            set control mode
*********************************************************************************************************/
INT8U MCP_CAN::mcp2515_setCANCTRL_Mode(const INT8U newmode)
{
    INT8U i;

    mcp2515_modifyRegister(MCP_CANCTRL, MODE_MASK, newmode);

    i = mcp2515_readRegister(MCP_CANCTRL);
    i &= MODE_MASK;

    if ( i == newmode ) 
    {
        return MCP2515_OK;
    }

    return MCP2515_FAIL;

}

/*********************************************************************************************************
** Function name:           mcp2515_configRate
** Descriptions:            set boadrate
*********************************************************************************************************/
INT8U MCP_CAN::mcp2515_configRate(const INT8U canSpeed)            
{
    INT8U set, cfg1, cfg2, cfg3;
    set = 1;
    switch (canSpeed) 
    {
        case (CAN_5KBPS):
        cfg1 = MCP_16MHz_5kBPS_CFG1;
        cfg2 = MCP_16MHz_5kBPS_CFG2;
        cfg3 = MCP_16MHz_5kBPS_CFG3;
        break;

        case (CAN_10KBPS):
        cfg1 = MCP_16MHz_10kBPS_CFG1;
        cfg2 = MCP_16MHz_10kBPS_CFG2;
        cfg3 = MCP_16MHz_10kBPS_CFG3;
        break;

        case (CAN_20KBPS):
        cfg1 = MCP_16MHz_20kBPS_CFG1;
        cfg2 = MCP_16MHz_20kBPS_CFG2;
        cfg3 = MCP_16MHz_20kBPS_CFG3;
        break;
        
        case (CAN_31K25BPS):
        cfg1 = MCP_16MHz_31k25BPS_CFG1;
        cfg2 = MCP_16MHz_31k25BPS_CFG2;
        cfg3 = MCP_16MHz_31k25BPS_CFG3;
        break;

        case (CAN_40KBPS):
        cfg1 = MCP_16MHz_40kBPS_CFG1;
        cfg2 = MCP_16MHz_40kBPS_CFG2;
        cfg3 = MCP_16MHz_40kBPS_CFG3;
        break;

        case (CAN_50KBPS):
        cfg1 = MCP_16MHz_50kBPS_CFG1;
        cfg2 = MCP_16MHz_50kBPS_CFG2;
        cfg3 = MCP_16MHz_50kBPS_CFG3;
        break;

        case (CAN_80KBPS):
        cfg1 = MCP_16MHz_80kBPS_CFG1;
        cfg2 = MCP_16MHz_80kBPS_CFG2;
        cfg3 = MCP_16MHz_80kBPS_CFG3;
        break;

        case (CAN_100KBPS):                                             /* 100KBPS                  */
        cfg1 = MCP_16MHz_100kBPS_CFG1;
        cfg2 = MCP_16MHz_100kBPS_CFG2;
        cfg3 = MCP_16MHz_100kBPS_CFG3;
        break;

        case (CAN_125KBPS):
        cfg1 = MCP_16MHz_125kBPS_CFG1;
        cfg2 = MCP_16MHz_125kBPS_CFG2;
        cfg3 = MCP_16MHz_125kBPS_CFG3;
        break;

        case (CAN_200KBPS):
        cfg1 = MCP_16MHz_200kBPS_CFG1;
        cfg2 = MCP_16MHz_200kBPS_CFG2;
        cfg3 = MCP_16MHz_200kBPS_CFG3;
        break;

        case (CAN_250KBPS):
        cfg1 = MCP_16MHz_250kBPS_CFG1;
        cfg2 = MCP_16MHz_250kBPS_CFG2;
        cfg3 = MCP_16MHz_250kBPS_CFG3;
        break;

        case (CAN_500KBPS):
        cfg1 = MCP_16MHz_500kBPS_CFG1;
        cfg2 = MCP_16MHz_500kBPS_CFG2;
        cfg3 = MCP_16MHz_500kBPS_CFG3;
        break;
        
        case (CAN_1000KBPS):
        cfg1 = MCP_16MHz_1000kBPS_CFG1;
        cfg2 = MCP_16MHz_1000kBPS_CFG2;
        cfg3 = MCP_16MHz_1000kBPS_CFG3;
        break;  

        default:
        set = 0;
        break;
    }

    if (set) {
        mcp2515_setRegister(MCP_CNF1, cfg1);
        mcp2515_setRegister(MCP_CNF2, cfg2);
        mcp2515_setRegister(MCP_CNF3, cfg3);
        return MCP2515_OK;
    }
    else {
        return MCP2515_FAIL;
    }
}

/*********************************************************************************************************
** Function name:           mcp2515_initCANBuffers
** Descriptions:            init canbuffers
*********************************************************************************************************/
void MCP_CAN::mcp2515_initCANBuffers(void)
{
    INT8U i, a1, a2, a3;
    
    INT8U std = 0;               
    INT8U ext = 1;
    INT32U ulMask = 0x00, ulFilt = 0x00;


    mcp2515_write_id(MCP_RXM0SIDH, ext, ulMask);			/*Set both masks to 0           */
    mcp2515_write_id(MCP_RXM1SIDH, ext, ulMask);			/*Mask register ignores ext bit */
    
                                                                        /* Set all filters to 0         */
    mcp2515_write_id(MCP_RXF0SIDH, ext, ulFilt);			/* RXB0: extended               */
    mcp2515_write_id(MCP_RXF1SIDH, std, ulFilt);			/* RXB1: standard               */
    mcp2515_write_id(MCP_RXF2SIDH, ext, ulFilt);			/* RXB2: extended               */
    mcp2515_write_id(MCP_RXF3SIDH, std, ulFilt);			/* RXB3: standard               */
    mcp2515_write_id(MCP_RXF4SIDH, ext, ulFilt);
    mcp2515_write_id(MCP_RXF5SIDH, std, ulFilt);

                                                                        /* Clear, deactivate the three  */
                                                                        /* transmit buffers             */
                                                                        /* TXBnCTRL -> TXBnD7           */
    a1 = MCP_TXB0CTRL;
    a2 = MCP_TXB1CTRL;
    a3 = MCP_TXB2CTRL;
    for (i = 0; i < 14; i++) {                                          /* in-buffer loop               */
        mcp2515_setRegister(a1, 0);
        mcp2515_setRegister(a2, 0);
        mcp2515_setRegister(a3, 0);
        a1++;
        a2++;
        a3++;
    }
    mcp2515_setRegister(MCP_RXB0CTRL, 0);
    mcp2515_setRegister(MCP_RXB1CTRL, 0);
}

/*********************************************************************************************************
** Function name:           mcp2515_init
** Descriptions:            init the device
*********************************************************************************************************/
INT8U MCP_CAN::mcp2515_init(const INT8U canSpeed)                       /* mcp2515init                  */
{

  INT8U res;

    mcp2515_reset();

    res = mcp2515_setCANCTRL_Mode(MODE_CONFIG);
    if(res > 0)
    {
#if DEBUG_MODE
      Serial.print("Enter setting mode fall\r\n"); 
#endif
      return res;
    }
#if DEBUG_MODE
    Serial.print("Enter setting mode success \r\n");
#endif

                                                                        /* set boadrate                 */
    if(mcp2515_configRate(canSpeed))
    {
#if DEBUG_MODE
      Serial.print("set rate fall!!\r\n");
#endif
      return res;
    }
#if DEBUG_MODE
    Serial.print("set rate success!!\r\n");
#endif

    if ( res == MCP2515_OK ) {

                                                                        /* init canbuffers              */
        mcp2515_initCANBuffers();

                                                                        /* interrupt mode               */
        mcp2515_setRegister(MCP_CANINTE, MCP_RX0IF | MCP_RX1IF);

#if (DEBUG_RXANY==1)
                                                                        /* enable both receive-buffers  */
                                                                        /* to receive any message       */
                                                                        /* and enable rollover          */
        mcp2515_modifyRegister(MCP_RXB0CTRL,
        MCP_RXB_RX_MASK | MCP_RXB_BUKT_MASK,
        MCP_RXB_RX_ANY | MCP_RXB_BUKT_MASK);
        mcp2515_modifyRegister(MCP_RXB1CTRL, MCP_RXB_RX_MASK,
        MCP_RXB_RX_ANY);
#else
                                                                        /* enable both receive-buffers  */
                                                                        /* to receive messages          */
                                                                        /* with std. and ext. identifie */
                                                                        /* rs                           */
                                                                        /* and enable rollover          */
        mcp2515_modifyRegister(MCP_RXB0CTRL,
        MCP_RXB_RX_MASK | MCP_RXB_BUKT_MASK,
        MCP_RXB_RX_STDEXT | MCP_RXB_BUKT_MASK );
        mcp2515_modifyRegister(MCP_RXB1CTRL, MCP_RXB_RX_MASK,
        MCP_RXB_RX_STDEXT);
#endif
                                                                        /* enter normal mode            */
        res = mcp2515_setCANCTRL_Mode(MODE_NORMAL);                                                                
        if(res)
        {
#if DEBUG_MODE        
          Serial.print("Enter Normal Mode Fall!!\r\n");
#endif           
          return res;
        }


#if DEBUG_MODE
          Serial.print("Enter Normal Mode Success!!\r\n");
#endif

    }
    return res;

}

/*********************************************************************************************************
** Function name:           mcp2515_write_id
** Descriptions:            write can id
*********************************************************************************************************/
void MCP_CAN::mcp2515_write_id( const INT8U mcp_addr, const INT8U ext, const INT32U id )
{
    uint16_t canid;
    INT8U tbufdata[4];

    canid = (uint16_t)(id & 0x0FFFF);

    if ( ext == 1) 
    {
        tbufdata[MCP_EID0] = (INT8U) (canid & 0xFF);
        tbufdata[MCP_EID8] = (INT8U) (canid >> 8);
        canid = (uint16_t)(id >> 16);
        tbufdata[MCP_SIDL] = (INT8U) (canid & 0x03);
        tbufdata[MCP_SIDL] += (INT8U) ((canid & 0x1C) << 3);
        tbufdata[MCP_SIDL] |= MCP_TXB_EXIDE_M;
        tbufdata[MCP_SIDH] = (INT8U) (canid >> 5 );
    }
    else 
    {
        tbufdata[MCP_SIDH] = (INT8U) (canid >> 3 );
        tbufdata[MCP_SIDL] = (INT8U) ((canid & 0x07 ) << 5);
        tbufdata[MCP_EID0] = 0;
        tbufdata[MCP_EID8] = 0;
    }
    mcp2515_setRegisterS( mcp_addr, tbufdata, 4 );
}

/*********************************************************************************************************
** Function name:           mcp2515_read_id
** Descriptions:            read can id
*********************************************************************************************************/
void MCP_CAN::mcp2515_read_id( const INT8U mcp_addr, INT8U* ext, INT32U* id )
{
    INT8U tbufdata[4];

    *ext = 0;
    *id = 0;

    mcp2515_readRegisterS( mcp_addr, tbufdata, 4 );

    *id = (tbufdata[MCP_SIDH]<<3) + (tbufdata[MCP_SIDL]>>5);

    if ( (tbufdata[MCP_SIDL] & MCP_TXB_EXIDE_M) ==  MCP_TXB_EXIDE_M ) 
    {
                                                                        /* extended id                  */
        *id = (*id<<2) + (tbufdata[MCP_SIDL] & 0x03);
        *id = (*id<<8) + tbufdata[MCP_EID8];
        *id = (*id<<8) + tbufdata[MCP_EID0];
        *ext = 1;
    }
}

/*********************************************************************************************************
** Function name:           mcp2515_write_canMsg
** Descriptions:            write msg
*********************************************************************************************************/
void MCP_CAN::mcp2515_write_canMsg( const INT8U buffer_sidh_addr)
{
    INT8U mcp_addr;
    mcp_addr = buffer_sidh_addr;
    mcp2515_setRegisterS(mcp_addr+5, m_nDta, m_nDlc );                  /* write data bytes             */
    if ( m_nRtr == 1)                                                   /* if RTR set bit in byte       */
    {
        m_nDlc |= MCP_RTR_MASK;  
    }
    mcp2515_setRegister((mcp_addr+4), m_nDlc );                        /* write the RTR and DLC        */
    mcp2515_write_id(mcp_addr, m_nExtFlg, m_nID );                     /* write CAN id                 */

}

/*********************************************************************************************************
** Function name:           mcp2515_read_canMsg
** Descriptions:            read message
*********************************************************************************************************/
void MCP_CAN::mcp2515_read_canMsg( const INT8U buffer_sidh_addr)        /* read can msg                 */
{
    INT8U mcp_addr, ctrl;

    mcp_addr = buffer_sidh_addr;

    mcp2515_read_id( mcp_addr, &m_nExtFlg,&m_nID );

    ctrl = mcp2515_readRegister( mcp_addr-1 );
    m_nDlc = mcp2515_readRegister( mcp_addr+4 );

    if ((ctrl & 0x08)) {
        m_nRtr = 1;
    }
    else {
        m_nRtr = 0;
    }

    m_nDlc &= MCP_DLC_MASK;
    mcp2515_readRegisterS( mcp_addr+5, &(m_nDta[0]), m_nDlc );
}

/*********************************************************************************************************
** Function name:           sendMsg
** Descriptions:            send message
*********************************************************************************************************/
void MCP_CAN::mcp2515_start_transmit(const INT8U mcp_addr)              /* start transmit               */
{
    mcp2515_modifyRegister( mcp_addr-1 , MCP_TXB_TXREQ_M, MCP_TXB_TXREQ_M );
}

/*********************************************************************************************************
** Function name:           sendMsg
** Descriptions:            send message
*********************************************************************************************************/
INT8U MCP_CAN::mcp2515_getNextFreeTXBuf(INT8U *txbuf_n)                 /* get Next free txbuf          */
{
    INT8U res, i, ctrlval;
    INT8U ctrlregs[MCP_N_TXBUFFERS] = { MCP_TXB0CTRL, MCP_TXB1CTRL, MCP_TXB2CTRL };

    res = MCP_ALLTXBUSY;
    *txbuf_n = 0x00;

                                                                        /* check all 3 TX-Buffers       */
    for (i=0; i<MCP_N_TXBUFFERS; i++) {
        ctrlval = mcp2515_readRegister( ctrlregs[i] );
        if ( (ctrlval & MCP_TXB_TXREQ_M) == 0 ) {
            *txbuf_n = ctrlregs[i]+1;                                   /* return SIDH-address of Buffe */
                                                                        /* r                            */
            res = MCP2515_OK;
            return res;                                                 /* ! function exit              */
        }
    }
    return res;
}

/*********************************************************************************************************
** Function name:           set CS
** Descriptions:            init CS pin and set UNSELECTED
*********************************************************************************************************/
void MCP_CAN::Initialize(INT8U _CS)
{
    SPICS = _CS;
    pinMode(SPICS, OUTPUT);
    MCP2515_UNSELECT();
}

/*********************************************************************************************************
** Function name:           init
** Descriptions:            init can and set speed
*********************************************************************************************************/
INT8U MCP_CAN::begin(INT8U speedset)
{
    INT8U res;

    SPI.begin();
    res = mcp2515_init(speedset);
    if (res == MCP2515_OK) return CAN_OK;
    else return CAN_FAILINIT;
}

/*********************************************************************************************************
** Function name:           init_Mask
** Descriptions:            init canid Masks
*********************************************************************************************************/
INT8U MCP_CAN::init_Mask(INT8U num, INT8U ext, INT32U ulData)
{
    INT8U res = MCP2515_OK;
#if DEBUG_MODE
    Serial.print("Begin to set Mask!!\r\n");
#endif
    res = mcp2515_setCANCTRL_Mode(MODE_CONFIG);
    if(res > 0){
#if DEBUG_MODE
    Serial.print("Enter setting mode fall\r\n"); 
#endif
  return res;
}
    
    if (num == 0){
        mcp2515_write_id(MCP_RXM0SIDH, ext, ulData);

    }
    else if(num == 1){
        mcp2515_write_id(MCP_RXM1SIDH, ext, ulData);
    }
    else res =  MCP2515_FAIL;
    
    res = mcp2515_setCANCTRL_Mode(MODE_NORMAL);
    if(res > 0){
#if DEBUG_MODE
    Serial.print("Enter normal mode fall\r\n"); 
#endif
    return res;
  }
#if DEBUG_MODE
    Serial.print("set Mask success!!\r\n");
#endif
    return res;
}

/*********************************************************************************************************
** Function name:           init_Filt
** Descriptions:            init canid filters
*********************************************************************************************************/
INT8U MCP_CAN::init_Filt(INT8U num, INT8U ext, INT32U ulData)
{
    INT8U res = MCP2515_OK;
#if DEBUG_MODE
    Serial.print("Begin to set Filter!!\r\n");
#endif
    res = mcp2515_setCANCTRL_Mode(MODE_CONFIG);
    if(res > 0)
    {
#if DEBUG_MODE
      Serial.print("Enter setting mode fall\r\n"); 
#endif
      return res;
    }
    
    switch( num )
    {
        case 0:
        mcp2515_write_id(MCP_RXF0SIDH, ext, ulData);
        break;

        case 1:
        mcp2515_write_id(MCP_RXF1SIDH, ext, ulData);
        break;

        case 2:
        mcp2515_write_id(MCP_RXF2SIDH, ext, ulData);
        break;

        case 3:
        mcp2515_write_id(MCP_RXF3SIDH, ext, ulData);
        break;

        case 4:
        mcp2515_write_id(MCP_RXF4SIDH, ext, ulData);
        break;

        case 5:
        mcp2515_write_id(MCP_RXF5SIDH, ext, ulData);
        break;

        default:
        res = MCP2515_FAIL;
    }
    
    res = mcp2515_setCANCTRL_Mode(MODE_NORMAL);
    if(res > 0)
    {
#if DEBUG_MODE
      Serial.print("Enter normal mode fall\r\nSet filter fail!!\r\n"); 
#endif
      return res;
    }
#if DEBUG_MODE
    Serial.print("set Filter success!!\r\n");
#endif
    
    return res;
}

/*********************************************************************************************************
** Function name:           setMsg
** Descriptions:            set can message, such as dlc, id, dta[] and so on
*********************************************************************************************************/
INT8U MCP_CAN::setMsg(INT32U id, INT8U ext, INT8U len, INT8U *pData)
{
    int i = 0;
    m_nExtFlg = ext;
    m_nID     = id;
    m_nDlc    = len;
    for(i = 0; i<MAX_CHAR_IN_MESSAGE; i++)
    m_nDta[i] = *(pData+i);
    return MCP2515_OK;
}

/*********************************************************************************************************
** Function name:           clearMsg
** Descriptions:            set all message to zero
*********************************************************************************************************/
INT8U MCP_CAN::clearMsg()
{
    m_nID       = 0;
    m_nDlc      = 0;
    m_nExtFlg   = 0;
    m_nRtr      = 0;
    m_nfilhit   = 0;
    for(int i = 0; i<m_nDlc; i++ )
      m_nDta[i] = 0x00;

    return MCP2515_OK;
}

/*********************************************************************************************************
** Function name:           sendMsg
** Descriptions:            send message
*********************************************************************************************************/
INT8U MCP_CAN::sendMsg()
{
    INT8U res, res1, txbuf_n;
    uint16_t uiTimeOut = 0;

    do {
        res = mcp2515_getNextFreeTXBuf(&txbuf_n);                       /* info = addr.                 */
        uiTimeOut++;
    } while (res == MCP_ALLTXBUSY && (uiTimeOut < TIMEOUTVALUE));

    if(uiTimeOut == TIMEOUTVALUE) 
    {   
        return CAN_GETTXBFTIMEOUT;                                      /* get tx buff time out         */
    }
    uiTimeOut = 0;
    mcp2515_write_canMsg( txbuf_n);
    mcp2515_start_transmit( txbuf_n );
    do
    {
        uiTimeOut++;        
        res1= mcp2515_readRegister(txbuf_n);  			                /* read send buff ctrl reg 	*/
        res1 = res1 & 0x08;                               		
    }while(res1 && (uiTimeOut < TIMEOUTVALUE));   
    if(uiTimeOut == TIMEOUTVALUE)                                       /* send msg timeout             */	
    {
        return CAN_SENDMSGTIMEOUT;
    }
    return CAN_OK;

}

/*********************************************************************************************************
** Function name:           sendMsgBuf
** Descriptions:            send buf
*********************************************************************************************************/
INT8U MCP_CAN::sendMsgBuf(INT32U id, INT8U ext, INT8U len, INT8U *buf)
{
    setMsg(id, ext, len, buf);
    sendMsg();
}

/*********************************************************************************************************
** Function name:           readMsg
** Descriptions:            read message
*********************************************************************************************************/
INT8U MCP_CAN::readMsg()
{
    INT8U stat, res;

    stat = mcp2515_readStatus();

    if ( stat & MCP_STAT_RX0IF )                                        /* Msg in Buffer 0              */
    {
        mcp2515_read_canMsg( MCP_RXBUF_0);
        mcp2515_modifyRegister(MCP_CANINTF, MCP_RX0IF, 0);
        res = CAN_OK;
    }
    else if ( stat & MCP_STAT_RX1IF )                                   /* Msg in Buffer 1              */
    {
        mcp2515_read_canMsg( MCP_RXBUF_1);
        mcp2515_modifyRegister(MCP_CANINTF, MCP_RX1IF, 0);
        res = CAN_OK;
    }
    else 
    {
        res = CAN_NOMSG;
    }
    return res;
}

/*********************************************************************************************************
** Function name:           readMsgBuf
** Descriptions:            read message buf
*********************************************************************************************************/
INT8U MCP_CAN::readMsgBuf(INT8U *len, INT8U buf[])
{
    readMsg();
    *len = m_nDlc;
    for(int i = 0; i<m_nDlc; i++)
    {
      buf[i] = m_nDta[i];
    }
}

/*********************************************************************************************************
** Function name:           checkReceive
** Descriptions:            check if got something
*********************************************************************************************************/
INT8U MCP_CAN::checkReceive(void)
{
    INT8U res;
    res = mcp2515_readStatus();                                         /* RXnIF in Bit 1 and 0         */
    if ( res & MCP_STAT_RXIF_MASK ) 
    {
        return CAN_MSGAVAIL;
    }
    else 
    {
        return CAN_NOMSG;
    }
}

/*********************************************************************************************************
** Function name:           checkError
** Descriptions:            if something error
*********************************************************************************************************/
INT8U MCP_CAN::checkError(void)
{
    INT8U eflg = mcp2515_readRegister(MCP_EFLG);

    if ( eflg & MCP_EFLG_ERRORMASK ) 
    {
        return CAN_CTRLERROR;
    }
    else 
    {
        return CAN_OK;
    }
}

/*********************************************************************************************************
** Function name:           getCanId
** Descriptions:            when receive something ,u can get the can id!!
*********************************************************************************************************/
INT32U MCP_CAN::getCanId(void)
{
    return m_nID;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

bool UCAN_UCANMSGStack::IsMessageEmpty(int n)
{
	if (n >= 0 && n < UCAN_MSGStack_Size && InUse[n] == true)
	{
		return false;
	}
	else
	{
		return true;
	};
};

UCANMessage UCAN_UCANMSGStack::FetchMessage(int n)
{
	if (n >= 0 && n < UCAN_MSGStack_Size && InUse[n] == true)
	{
		return Data[n];
	}
	else
	{
		return UCAN_EmptyMessage();
	};
};

void UCAN_UCANMSGStack::StoreMessage(int n, UCANMessage MSG)
{
	if (n >= 0 && n < UCAN_MSGStack_Size)
	{
		Data[n] = MSG;
		InUse[n] = true;
	};
};

void UCAN_UCANFeedStack::TrackID(int id)
{
	int n;
	
	if (IsIDTracked(id) == true)
	{
		return;
	};
	
	n = FindIDPosition(-1); //-1 is put into available slots
	
	if (n >= 0)
	{
		TrackingID[n] = id;
	};
};

void UCAN_UCANFeedStack::unTrackID(int id)
{
	int n = FindIDPosition(id);
	
	if (IsIDTracked(id) == false)
	{
		return;
	};
	
	TrackingID[n] = -1;
};

void UCAN_UCANFeedStack::SetIDTracking(int id, bool mode)
{
	if (mode == false)
	{
		unTrackID(id);
	}
	else if (mode == true)
	{
		TrackID(id);
	};
};

bool UCAN_UCANFeedStack::IsIDTracked(int id)
{
	if (FindIDPosition(id) >= 0)
	{
		return true;
	};
	
	return false;
};

int UCAN_UCANFeedStack::FindIDPosition(int id)
{
	int c = 0;
	while (c <= UCAN_FeedStack_Size)
	{
		if (id == TrackingID[c])
		{
			return c;
		};
		c ++;
	};
	
	return -1; //Could be a 0... This might break stuff at some point.
};

void UCAN_UCANFeedStack::Initialize(void)
{
	memset(&TrackingID, -1, UCAN_FeedStack_Size);
};

void UCAN_UCANFeedStack::Empty(void)
{
	Initialize(); //Not a dynamem implementation, so good enough
};


int UCAN_UCANMSGStack::GetNextEmptySlot(void)
{
	int c = 0;
	
	while (c <= UCAN_MSGStack_Size)
	{
		if (InUse[c] == false)
		{
			return c;
		};
		
		c ++;
	};
	
	return -1;
};

int UCAN_UCANMSGStack::GetAvailableSlots(void)
{
	int c = 0;
	int n = 0;
	
	while (c <= UCAN_MSGStack_Size)
	{
		if (InUse[c] == false)
		{
			n ++;
		};
		
		c ++;
	};
	
	return n;
};

void UCAN_UCANMSGStack::Empty(void)
{
	Initialize(); //Also not dynamem, so also good enough
};

void UCAN_UCANMSGStack::Initialize(void)
{
	int c = 0;
	
	while (c <= UCAN_MSGStack_Size)
	{
		Data[c] = UCAN_EmptyMessage();
		InUse[c] = false;
		
		c ++;
	};
};

bool UCAN_UCANHandler::CAN_IsMessagePending(void)
{
	// On Arduino/AVR we get a pin written to 0 whenever we have CAN data waiting to be read.
	if (digitalRead(UCAN_MCPIntPin) == LOW)
	{
		DebugMSG(UCAN_Debug_CANPending, 2);
		return true;
	};
	
	if (MCPCANBus.checkReceive() == CAN_MSGAVAIL)
	{
		DebugMSG(UCAN_Debug_CANPending, 1);
		return true;
	}
	else
	{
		DebugMSG(UCAN_Debug_CANPending, -1);
		return false;
	};
};

UCANMessage UCAN_UCANHandler::CAN_FetchMsgFromCAN(void)
{
	uint8_t c = 0;
	uint8_t rxBuf[8];
	uint8_t rxLen = 0;
	uint8_t rxAddress = 0;
	UCANMessage ReturnMSG;
	
	DebugMSG(UCAN_Debug_CANFetchMSG);
	
	ReturnMSG = UCAN_EmptyMessage();
	
	if (CAN_IsMessagePending() == true)
	{
		MCPCANBus.readMsgBuf(&rxLen, rxBuf);
		
		ReturnMSG.Address = MCPCANBus.getCanId();
		ReturnMSG.Channel = rxBuf[0];
		while (c <= 7)
		{
			ReturnMSG.Data[c] = rxBuf[c + 1];
			c ++;
		};
	};
	
	return ReturnMSG;
};

void UCAN_UCANHandler::CAN_SendMSG(UCANMessage MSG)
{
	uint8_t c = 0;
	uint8_t txBuff[8];
	
	txBuff[0] = MSG.Channel;
	while (c <= 6)
	{
		txBuff[c + 1] = MSG.Data[c];
		c ++;
	};
	
	MCPCANBus.sendMsgBuf(CAN_ID, 0, 8, txBuff);
};

// Michael's note: could we make DebugMSG a macro like so:
//
// #ifdef RELEASE
//		#define DEBUG_MSG(msg, data, extdata) /* nothing */
// #else
//		#define DEBUG_MSG(msg, data, extdata) DebugMSG(...)
// #endif
//
// Responose: GCC optimization should remove these empty calls anyway... Style consideration?

// Michael's note: use C++ templates to combine these functions into one?
// Response: Possible. Templates untested in current build environment. Left as exercise to reader.

void DebugMSG(int16_t MSG)
{
	#ifndef RELEASE
		Serial.print(UCAN_Debug_DebugSTRID);
		Serial.println(MSG);
	#endif
};

void DebugMSG(int16_t MSG, int16_t data)
{
	#ifndef RELEASE
		Serial.print(UCAN_Debug_DebugSTRID);
		Serial.print(MSG);
		Serial.print(" ");
		Serial.println(data);
	#endif
};

void DebugMSG(int16_t MSG, float data)
{
	#ifndef RELEASE
		Serial.print(UCAN_Debug_DebugSTRID);
		Serial.print(MSG);
		Serial.print(" ");
		Serial.println(data);
	#endif
};

void DebugMSG(int16_t MSG, int16_t data, int16_t extdata)
{
	#ifndef RELEASE
		Serial.print(UCAN_Debug_DebugSTRID);
		Serial.print(MSG);
		Serial.print(" ");
		Serial.print(data);
		Serial.print(" ");
		Serial.println(extdata);
	#endif
};

void DebugMSG(int16_t MSG, int16_t data, uint16_t extdata)
{
	#ifndef RELEASE
		Serial.print(UCAN_Debug_DebugSTRID);
		Serial.print(MSG);
		Serial.print(" ");
		Serial.print(data);
		Serial.print(" ");
		Serial.println(extdata);
	#endif
};

void DebugMSG(int16_t MSG, int16_t data, int32_t extdata)
{
	#ifndef RELEASE
		Serial.print(UCAN_Debug_DebugSTRID);
		Serial.print(MSG);
		Serial.print(" ");
		Serial.print(data);
		Serial.print(" ");
		Serial.println(extdata);
	#endif
};

void DebugMSG(int16_t MSG, int16_t data, uint32_t extdata)
{
	#ifndef RELEASE
		Serial.print(UCAN_Debug_DebugSTRID);
		Serial.print(MSG);
		Serial.print(" ");
		Serial.print(data);
		Serial.print(" ");
		Serial.println(extdata);
	#endif
};

void DebugMSG(UCANMessage MSG)
{
	#ifndef RELEASE
		uint8_t c = 0;
		
		Serial.print(UCAN_Debug_DebugSTRID);
		Serial.print(" ");
		Serial.print(MSG.Address);
		Serial.print(" ");
		Serial.print(MSG.Channel);
		while (c < 7)
		{
			Serial.print(" ");
			Serial.print(MSG.Data[c]);
			
			c++;
		};
	Serial.println("");
	#endif
};

void DebugMSG(int MSG, int data, float extdata)
{
	#ifndef RELEASE
		Serial.print(UCAN_Debug_DebugSTRID);
		Serial.print(MSG);
		Serial.print(" ");
		Serial.print(data);
		Serial.print(" ");
		Serial.println(extdata);
	#endif
};

void UCAN_UCANHandler::SetID(int ID)
{
	DebugMSG(UCAN_Debug_Call_SetID, ID);
	if (ID > 0 && ID <= 2047)
	{
		CAN_ID = ID;
	}
	else
	{
		CAN_ID = 2047;
	};
};

uint8_t UCAN_UCANHandler::NewStreamDUID(void)
{
	DebugMSG(UCAN_Debug_Stub);
};

void UCAN_UCANHandler::IntProc_RXHandler(void)
{
	DebugMSG(UCAN_Debug_Stub);
};

void UCAN_UCANHandler::SendMessage(UCANMessage msg)
{
	DebugMSG(UCAN_Debug_TxMSG);
	CAN_SendMSG(msg);
};

void UCAN_UCANHandler::Chan0_Announce(uint8_t flag)
{
	UCANMessage MSG;
	
	MSG.Channel = 0;
	MSG.Data[0] = 0; //TypeID 0 is an announce
	MSG.Data[1] = flag;
	
	MSG.Data[5] = BytesFromi16(BootSecret, 0);
	MSG.Data[6] = BytesFromi16(BootSecret, 1);
	
	SendMessage(MSG);
	
	DebugMSG(UCAN_Debug_Chan0Announce, flag);
};

void UCAN_UCANHandler::Chan0_Request(uint8_t flag, int data)
{
	UCANMessage MSG;
	
	union
	{
		int i16;
		uint8_t b[2];
	};
	
	i16 = data;
	
	MSG.Channel = 0;
	MSG.Data[0] = 1; //TypeID 1 is a request
	MSG.Data[1] = flag;
	MSG.Data[2] = b[0];
	MSG.Data[3] = b[1];
	
	// Michael's note: refactor this.
	MSG.Data[5] = BytesFromi16(BootSecret, 0);
	MSG.Data[6] = BytesFromi16(BootSecret, 1);
	
	SendMessage(MSG);
	
	DebugMSG(UCAN_Debug_Chan0Request, flag, data);
};

void UCAN_UCANHandler::Initialize(void)
{
	int c = 0;
	int t = 0;
	int x = 0;
	
	delay(50);
	pinMode(UCAN_MCPResetPin, OUTPUT);
	digitalWrite(UCAN_MCPResetPin, LOW);
	delay(100);
	digitalWrite(UCAN_MCPResetPin, HIGH);
	
	if (Initialized == true)
	{
		return;
	};
	DebugMSG(UCAN_Debug_Call_Init);
	
	delay(50);
	pinMode(UCAN_MCPResetPin, OUTPUT);
	digitalWrite(UCAN_MCPResetPin, LOW);
	delay(100);
	digitalWrite(UCAN_MCPResetPin, HIGH);
	
	//We will need a unique secret to this device for identification
	randomSeed(analogRead(A6));
	t = random(1000);
	
	// Michael's note: check distribution of analogRead calls (returns integers 1-1023)
	// and try and change the loop into a maths function to ensure an even distribution.
	// Is there a more foolproof way to solve the "force shutdown" problem of CAN nodes by ID, where
	// the node shuts down itself?
	
	//Response: Current mechanism appears to work. Not a priority for current stages of development.
	//			Better implementation welcome if offered and tested. This could reduce boot times considerably.
	
	//We should pick up some electrical noise...
	//So we will get lots of it and use it to seed the RNG
	while (c < t)
	{
		x = analogRead(A6);
		randomSeed(x);
		delayMicroseconds(x);
		
		c ++;
	};
	
	DebugMSG(UCAN_Debug_Call_Init, 1);
	MCPCANBus.Initialize(UCAN_MCPCSPin);
	
	if (CAN_ID <= 0)
	{
		DebugMSG(UCAN_Debug_ERROR_BadID);
		Empty();
		return;
	};
	DebugMSG(UCAN_Debug_Call_Init, 10);
	
	if (MCPCANBus.begin(CAN_500KBPS) != CAN_OK)
	{
		DebugMSG(UCAN_Debug_ERROR_BadCANInit);
	};
	
	//Can bus initialization can vary in time...
	//Get a more random number...
	while (c > 10)
	{
		c = random(500);
	};
	BootSecret = random(65535); //It will overflow randomly. Which is okay!
	DebugMSG(UCAN_Debug_BootSecret, BootSecret);
	
	//Let other devices know we are joining the bus
	Chan0_Announce(1); //Send an announce on Flag 0, Data = 0
	
	//See implementation note above
	//RequestCanID(2047);
	
	pinMode(UCAN_MCPIntPin, INPUT);	
	Initialized = true;
	
	//NOTE: Interrupt based UCAN scheduled to be re-enabled for February 2016.

	if (UCANCallMode == UCAN_CallMode_FullAuto)
	{
		//Attach UCANHandler.main() to CAN interrupt
		//attachInterrupt(0, UCANCallInterrupt_Default, CHANGE); //0 = INT0 = pin 2
	};
	
	if (UCANCallMode == UCAN_CallMode_ContAuto)
	{
		//Attach UCANHandler.main() to timer interrupt
	};
	
	if (UCANCallMode == UCAN_CallMode_FullManual)
	{
		//Do not attach UCANHandler.main() to anything, ignore interrupt
	};
	
	if (UCANCallMode == UCAN_CallMode_ContManual)
	{
		//UCANHandler.main() attached by user to interrupt
	};
	
	DebugMSG(UCAN_Debug_Call_Init, 100);
};

void UCAN_UCANHandler::Empty(void)
{
	DebugMSG(UCAN_Debug_Stub);
};

void UCANCallInterrupt_Default()
{
	UCAN.Main();
};

void UCAN_UCANHandler::HandlerMode(int Mode)
{
	DebugMSG(UCAN_Debug_Call_HandlerMode, Mode);
	
	if (Mode > 0)
	{
		UCANCallMode = Mode;
	};
	
};

void UCAN_UCANHandler::FeedMode(int Mode)
{
	DebugMSG(UCAN_Debug_Call_FeedMode, Mode);
	if (Mode >= 0)
	{
		UCANFeedMode = Mode;
	};
};

void UCAN_UCANHandler::StackMode(int Mode)
{
	DebugMSG(UCAN_Debug_Call_StackMode, Mode);
	if (Mode >= 0)
	{
		UCANStackMode = Mode;
	};
};

void UCAN_UCANHandler::WatchValue_f32(int ValueID, float* f32Pointer)
{
//	DebugMSG(UCAN_Debug_WatchCall_f32, ValueID, (uint16_t)f32Pointer);
	TrackingStack.TrackID(ValueID, f32Pointer);
};

void UCAN_UCANHandler::SendValue_l32(int16_t Value, uint32_t l32)
{
	UCANMessage msg;
	
	DebugMSG(UCAN_Debug_SendCall_l32, Value, l32);
	
	msg.Address = CAN_ID;
	msg.Channel = 1;
	msg.Data[0] = UCAN_ChanType_1_l32;
	msg.Data[1] = BytesFromi16(Value, 0);
	msg.Data[2] = BytesFromi16(Value, 1);

	for (uint8_t i = 0; i < 4; i ++) {
		msg.Data[i + 3] = BytesFroml32(l32, i);
	};
	
	SendMessage(msg);
};

void UCAN_UCANHandler::SendValue_i32(int16_t Value, int32_t i32)
{
	UCANMessage msg;
	
	DebugMSG(UCAN_Debug_SendCall_i32, Value, i32);
	
	msg.Address = CAN_ID;
	msg.Channel = 1;
	msg.Data[0] = UCAN_ChanType_1_i32;
	msg.Data[1] = BytesFromi16(Value, 0);
	msg.Data[2] = BytesFromi16(Value, 1);
	
	for (uint8_t i = 0; i < 4; i ++) {
		msg.Data[i + 3] = BytesFromi32(i32, i);
	};

	SendMessage(msg);
};

void UCAN_UCANHandler::SendValue_f32(int16_t Value, float f32)
{
	UCANMessage msg;
	
	DebugMSG(UCAN_Debug_SendCall_f32, Value, f32);
	
	msg.Address = CAN_ID;
	msg.Channel = 1;
	msg.Data[0] = UCAN_ChanType_1_f32;
	msg.Data[1] = BytesFromi16(Value, 0);
	msg.Data[2] = BytesFromi16(Value, 1);
	
	for (uint8_t i = 0; i < 4; i ++) {
		msg.Data[i + 3] = BytesFromf32(f32, i);
	};

	
	SendMessage(msg);
};

void UCAN_UCANHandler::SendValue_i16(int16_t Value, int16_t i16)
{
	UCANMessage msg;
	
	DebugMSG(UCAN_Debug_SendCall_i16, Value, i16);
	
	msg.Address = CAN_ID;
	msg.Channel = 1;
	msg.Data[0] = UCAN_ChanType_1_i16;
	msg.Data[1] = BytesFromi16(Value, 0);
	msg.Data[2] = BytesFromi16(Value, 1);
	msg.Data[3] = BytesFromi16(i16, 0);
	msg.Data[4] = BytesFromi16(i16, 1);
	
	SendMessage(msg);
};

void UCAN_UCANHandler::SendValue_l16(int16_t Value, uint16_t l16)
{
	UCANMessage msg;
	
	DebugMSG(UCAN_Debug_SendCall_l16, Value, l16);
	
	msg.Address = CAN_ID;
	msg.Channel = 1;
	msg.Data[0] = UCAN_ChanType_1_l16;
	msg.Data[1] = BytesFromi16(Value, 0);
	msg.Data[2] = BytesFromi16(Value, 1);
	msg.Data[3] = BytesFroml16(l16, 0);
	msg.Data[4] = BytesFroml16(l16, 1);
	
	SendMessage(msg);
};

bool UCAN_UCANHandler::IsMessagePending(void)
{
	int c = 0;
	
	while (c < UCAN_MSGStack_Size)
	{
		if (MSGStack.InUse[c] == true)
		{
			return true;
		};
		
		c ++;
	};
	
	return false;
};

UCANMessage UCAN_UCANHandler::GetNextMessage(void)
{
	UCANMessage Buffer;
	
	int c = 0;
	while (c < UCAN_MSGStack_Size)
	{
		if (MSGStack.InUse[c] == true)
		{
			Buffer = MSGStack.Data[c];
			if (UCANStackMode == UCAN_StackMode_Auto)
			{
				MSGStack.InUse[c] = false;
				MSGStack.Data[c] = UCAN_EmptyMessage();
				DebugMSG(UCAN_Debug_Call_GetNext_Clear, c);
			};
			
			return Buffer;
		};
		
		c++;
	};
};

void UCAN_UCANHandler::FetchNewMessages(void)
{
	int slot = 0;
	int c = 0;
	UCANMessage CANMsg;
	
	/*
	 * If we have a message waiting to be read from the CAN bus,
	 * 	check if we have available storage memory,
	 * 	allocate a storage slot for later processing,
	 * 	and read the message from the CAN bus via UCANHAL.
	*/
	if (CAN_IsMessagePending() == true)
	{		
		slot = MSGStack.GetNextEmptySlot();
		MSGStack.StoreMessage(slot, CAN_FetchMsgFromCAN());
		MSGStack.InUse[c] = true;
		DebugMSG(UCAN_Debug_StoreMSG, slot);
	}
	else {
		DebugMSG(UCAN_Debug_NoMSG);
		return;
	};
	
	if (CAN_IsMessagePending() == true && MSGStack.GetAvailableSlots() > 0)
	{
		FetchNewMessages();
	};
};

void UCAN_UCANHandler::MSGProcessor_DataServ(UCANMessage MSG)
{
	float f32;
	int16_t i16;
	uint16_t l16;
	int16_t ID;
	uint32_t l32;
	int32_t i32;
	
	DebugMSG(UCAN_Debug_Process_Data, MSG.Data[0]);
	switch (MSG.Data[0])
	{
		case UCAN_ChanType_1_f32:
				ID = Bytes16Toi16(MSG.Data[1], MSG.Data[2]);
				f32 = Bytes32Tof32(MSG.Data[3], MSG.Data[4], MSG.Data[5], MSG.Data[6]);
				DebugMSG(UCAN_Debug_RECV_Ch1_DServ_f32, ID, f32);
				
				TrackingStack.UpdateIDValue(ID, f32);
			break;
			
		case UCAN_ChanType_1_l32:
				ID = Bytes16Toi16(MSG.Data[1], MSG.Data[2]);
				l32 = Bytes32Tol32(MSG.Data[3], MSG.Data[4], MSG.Data[5], MSG.Data[6]);
				DebugMSG(UCAN_Debug_RECV_Ch1_DServ_l32, ID, l32);
				
				TrackingStack.UpdateIDValue(ID, l32);
			break;
			
		case UCAN_ChanType_1_i32:
				ID = Bytes16Toi16(MSG.Data[1], MSG.Data[2]);
				i32 = Bytes32Toi32(MSG.Data[3], MSG.Data[4], MSG.Data[5], MSG.Data[6]);
				DebugMSG(UCAN_Debug_RECV_Ch1_DServ_i32, ID, i32);
				
				TrackingStack.UpdateIDValue(ID, i32);
			break;
		
		case UCAN_ChanType_1_l16:
				ID = Bytes16Toi16(MSG.Data[1], MSG.Data[2]);
				i16 = Bytes16Tol16(MSG.Data[3], MSG.Data[4]);
				DebugMSG(UCAN_Debug_RECV_Ch1_DServ_l16, ID, l16);
				
				TrackingStack.UpdateIDValue(ID, l16);
			break;
			
		case UCAN_ChanType_1_i16:
				ID = Bytes16Toi16(MSG.Data[1], MSG.Data[2]);
				i16 = Bytes16Toi16(MSG.Data[3], MSG.Data[4]);
				DebugMSG(UCAN_Debug_RECV_Ch1_DServ_i16, ID, i16);
				
				TrackingStack.UpdateIDValue(ID, i16);
			break;
			
		DebugMSG(UCAN_Debug_BadPKTFormat);
	};
};

void UCAN_UCANHandler::MSGProcessor_InfoServ(UCANMessage MSG)
{
	switch (MSG.Data[0])
	{
		case 0:
			break;
		case 1: //catch new devices joining the bus
				if (Bytes16Toi16(MSG.Data[5], MSG.Data[6]) != BootSecret) //check it is not our own announce
				{
					Chan0_Request(2, MSG.Address); //Send halt request to origin of packet
				};
			break;
	};
};

void UCAN_UCANHandler::MSGDispatcher(UCANMessage MSG)
{
	DebugMSG(UCAN_Debug_MSGDispatch, MSG.Channel);
	switch (MSG.Channel)
	{
		case 0:
				MSGProcessor_InfoServ(MSG);
			break;
			
		case 1:
				MSGProcessor_DataServ(MSG);
			break;
	};
};

void UCAN_UCANHandler::ProcessMSGTriggers(void)
{
	int c = 0;
	
	while (c < UCAN_FeedStack_Size)
	{
		if (MSGStack.IsMessageEmpty(c) == false)
		{
			MSGDispatcher(MSGStack.FetchMessage(c));
			MSGStack.Data[c] = UCAN_EmptyMessage();
			MSGStack.InUse[c] = false;
			
			DebugMSG(UCAN_Debug_StackCallM_DEL, c);
		};
		
		c ++;
	};
};

void UCAN_UCANHandler::Main(void)
{
	if (Initialized == false)
	{
		return;
	};
	
	DebugMSG(UCAN_Debug_Call_Main);
	FetchNewMessages();
	if (UCANFeedMode != UCAN_FeedMode_Manual)
	{
		ProcessMSGTriggers();
	};
	//ProcessUCANOperations(); //Not yet implemented
};

UCANMessage UCAN_EmptyMessage(void)
{
	uint8_t c = 0;
	UCANMessage ReturnMSG;
	
	ReturnMSG.Channel = 0;
	ReturnMSG.Address = 0;
	
	// Michael's quick note- Am I being stupid, or does this only initialise the
	// ReturnMSG.Data[0] to zero? As c == 0, the while loop
	// continues, now c is 1... (condition fails, 1 <= 0 is false)?
	
	// Response: Good catch. Idiot programmer made doodoo. Now fixed with for loop style!
	
	for (uint8_t i = 0; i <= 6; i ++) {
		ReturnMSG.Data[i] = 0;
	};
	
	return ReturnMSG;
};

void UCAN_UCANWatchStack::Empty(void)
{
	int c = 0;
	while (c < UCAN_FeedStack_Size)
	{
		Target[c] = 0;
		TrackingID[c] = 0;
		
		c ++;
	};
};

void UCAN_UCANWatchStack::Initialize(void)
{
	Empty(); //Somewhat equivalent for now
};

int UCAN_UCANWatchStack::FindTargetPosition(float* LMem)
{
	int c = 0;
	while (c < UCAN_FeedStack_Size)
	{
		if (LMem == Target[c])
		{
			return c;
		};
		
		c ++;
	};
	
	return 0;
};

int UCAN_UCANWatchStack::FindEntry(int id, float* TMem)
{
	int c = 0;
	while (c < UCAN_FeedStack_Size)
	{
		if (id == TrackingID[c] && TMem == Target[c])
		{
			return c;
		};
		
		c ++;
	};
	
	return 0;
};

bool UCAN_UCANWatchStack::IsDuplicate(int id, float* TMem)
{
	int c = 0;
	while (c < UCAN_FeedStack_Size)
	{
		if (id == TrackingID[c] && TMem == Target[c])
		{
			return true;
		};
		
		c ++;
	};
	
	return false;
};

int UCAN_UCANWatchStack::FindEmptyPosition()
{
	int c = UCAN_FeedStack_Size;
	while (c >= 0)
	{
		c --;
		if (!Target[c])
		{
			DebugMSG(UCAN_Debug_StackCallW_FEMPT, c);
			return c;
		};
	};
	return 0; //Keeps things alive in case of bad code
	
	DebugMSG(UCAN_Debug_StackCallW_FEMPT, 0);
};

void UCAN_UCANWatchStack::TrackID(int id, float* TMem)
{
	int t = 0;
	
	if (IsDuplicate(id, TMem) == true)
	{
		return;
	};
	
	t = FindEmptyPosition();
	
	TrackingID[t] = id;
	Target[t] = TMem;
	
	DebugMSG(UCAN_Debug_StackCallW_ADD, t);
};

void UCAN_UCANWatchStack::unTrackID(int id, float* TMem)
{
	int t = 0;
	
	if (IsDuplicate(id, TMem) == false)
	{
		return;
	};
	
	t = FindEmptyPosition();
	
	TrackingID[t] = 0;
	Target[t] = 0;
	
	DebugMSG(UCAN_Debug_StackCallW_DEL, t);
};

void UCAN_UCANWatchStack::UpdateIDValue(int ID, float Value)
{
	int c = 0;
	DebugMSG(UCAN_Debug_UpdateFeedStack, ID, Value);
	while (c < UCAN_FeedStack_Size)
	{
		if (TrackingID[c] == ID && Target[c])
		{
			DebugMSG(UCAN_Debug_StackCallW_UPDAT, (int)Target[c], Value);
			*Target[c] = Value;
		};
		
		c ++;
	};
};

float Bytes32Tof32(uint8_t b32_1, uint8_t b32_2, uint8_t b32_3, uint8_t b32_4)
{
	union
	{
		float f32;
		uint8_t b[4];
	};
	
	b[0] = b32_1;
	b[1] = b32_2;
	b[2] = b32_3;
	b[3] = b32_4;
	
	return f32;
};

uint32_t Bytes32Tol32(uint8_t b32_1, uint8_t b32_2, uint8_t b32_3, uint8_t b32_4)
{
	union
	{
		uint32_t l32;
		uint8_t b[4];
	};
	
	b[0] = b32_1;
	b[1] = b32_2;
	b[2] = b32_3;
	b[3] = b32_4;
	
	return l32;
};

int32_t Bytes32Toi32(uint8_t b32_1, uint8_t b32_2, uint8_t b32_3, uint8_t b32_4)
{
	union
	{
		int32_t i32;
		uint8_t b[4];
	};
	
	b[0] = b32_1;
	b[1] = b32_2;
	b[2] = b32_3;
	b[3] = b32_4;
	
	return i32;
};

uint8_t BytesFromf32(float f, uint8_t ByteNumber)
{
	union
	{
		float f32;
		uint8_t b[4];
	};
	
	if (ByteNumber < 0) ByteNumber = 0;
	else if (ByteNumber > 3) ByteNumber = 3;
	
	f32 = f;
	return b[ByteNumber];
};

uint16_t Bytes16Tol16(uint8_t b16_1, uint8_t b16_2)
{
	union
	{
		uint16_t l16;
		uint8_t b[2];
	};
	
	b[0] = b16_1;
	b[1] = b16_2;
	
	return l16;
};

int16_t Bytes16Toi16(uint8_t b16_1, uint8_t b16_2)
{
	union
	{
		int16_t i16;
		uint8_t b[2];
	};
	
	b[0] = b16_1;
	b[1] = b16_2;
	
	return i16;
};

uint8_t BytesFromi16(int16_t i, uint8_t ByteNumber)
{
	union
	{
		int16_t i16;
		uint8_t b[2];
	};
	
	if (ByteNumber < 0) ByteNumber = 0;
	else if (ByteNumber >= 2) ByteNumber = 1;
	
	i16 = i;
	return b[ByteNumber];
};

uint8_t BytesFroml16(uint16_t i, uint8_t ByteNumber)
{
	union
	{
		uint16_t i16;
		uint8_t b[2];
	};
	
	if (ByteNumber < 0) ByteNumber = 0;
	else if (ByteNumber >= 2) ByteNumber = 1;
	
	i16 = i;
	return b[ByteNumber];
};

uint8_t BytesFromi32(int32_t i, uint8_t ByteNumber)
{
	union
	{
		int32_t i32;
		uint8_t b[4];
	};
	
	if (ByteNumber < 0) ByteNumber = 0;
	else if (ByteNumber >= 4) ByteNumber = 3;
	
	i32 = i;
	return b[ByteNumber];
};


uint8_t BytesFroml32(uint32_t i, uint8_t ByteNumber)
{
	union
	{
		uint32_t l32;
		uint8_t b[4];
	};
	
	if (ByteNumber < 0) ByteNumber = 0;
	else if (ByteNumber >= 4) ByteNumber = 3;
	
	l32 = i;
	return b[ByteNumber];
};

UCAN_UCANHandler UCAN;
