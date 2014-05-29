#ifndef COMMON_H
#define COMMON_H

/** 发送模式枚举值*/
typedef enum enum_send_mode
{
    ENUM_NO_ANSWER          = 0,               /**<无需应答方式,如心跳*/
    ENUM_NEED_ANSWER        = 1,               /**<需要应答方式,如请求*/
}ENUM_SEND_MODE;

typedef struct ec_header
{
    quint16 load_len;     //!<两字节具体消息串长度
    quint16 msgType;      //!<两字节MsgType
    quint16 msgId;        //!<两字节MsgID
    quint16 sevId;        //!<两字节从服务端发过来的用
}EC_HEADER;

typedef enum enum_msg_type
{
    ENUM_ANSWER = 3000,                                 //响应包
        ENUM_QUERY_RDT_SELECTED_3001 = 3001,				//RDT 指令选中消息,用于优先级为2的指令取消
        ENUM_QUERY_RDT_TASKLIST_3002 = 3002,				//向服务器请求指令清单
        ENUM_ANSWER_FROM_RDT = 3100,                        //响应包

        //处理
        ENUM_QUERY_RDT_LOGIN_4001 = 4001,					//RDT用户登录
        ENUM_QUERY_RDT_STATUS_4002 = 4002,					//RDT 心跳或可用状态
        ENUM_QUERY_RDT_SELECTED_4003 = 4003,				//RDT 指令选中消息
        ENUM_QUERY_RDT_POINT_AND_VESSEL_LOGIN_4004 = 4004,	//RDT 理货输入工作点和船舶参考号登录
        ENUM_QUERY_RDT_LOAD_OR_UNLOAD_GOODS_4005 = 4005,	//RDT 装车、卸车、装船、卸船指令确认消息
        ENUM_QUERY_RDT_STATUS_UPDATE_4006 = 4006,			//RDT状态更新消息 Available OR UnAvailable
        ENUM_QUERY_RDT_SHIFT_BOX_4007 = 4007,				//RDT翻箱指令确认消息
        ENUM_QUERY_RDT_LOGOUT_4008 = 4008,					//RDT用户退出请求
        ENUM_QUERY_RDT_MODIFY_BOX_4009 = 4009,				//RDT修改箱信息消息？？修改箱信息内容RDT给
        ENUM_QUERY_RDT_SAVE_PAGE_INFO_4010 = 4010,			//当前终端页面状态信息保存消息？？
        ENUM_QUERY_RDT_CREATE_BOX_4017 = 4017,				//RDT岸边理货新增溢卸箱报文
        ENUM_QUERY_RDT_ADJUST_SECTION_VIEW_4018 = 4018,			//RDT场地机械调整箱位报文

        //查询
        ENUM_QUERY_RDT_PARAM_4000 = 4000,					//查询RDT参数
        //ENUM_QUERY_RDT_SINGLE_BOX_LOAD_OR_UNLOAD_4052 = 4052,//RDT查询单箱装卸船指令
        //4052报文ID改成 4020
        ENUM_QUERY_RDT_SINGLE_BOX_LOAD_OR_UNLOAD_4020 = 4020,//RDT查询单箱装卸船指令

        ENUM_QUERY_RDT_RESET_CAR_4021 = 4021,//RDT查询单箱装卸船指令
        ENUM_QUERY_RDT_DMS_EXCHANGE_4022 = 4022,//RDT DMS手动换箱请求，add by xzg 2011.2.25
        ENUM_QUERY_RDT_RESEVER_CTN_DOOR_4023 = 4023,  //RDT调箱门指令确认请求
        ENUM_QUERY_RDT_ENTER_HCPL_4024 = 4024,			//RDT录入舱盖板销子箱
        ENUM_QUERY_RDT_SWITCH_BAY_4027 = 4027,				//RDT室内理货当前作业贝切换
        ENUM_QUERY_RDT_RH_CHANGE_WORK_POINT_4028 = 4028,				//RDT室内理货切换工作点
        ENUM_QUERY_RDT_RH_MODIFY_PASSWORD_4029 = 4029,      //RDT室内理货修改密码
        ENUM_QUERY_RDT_WORK_LIST_4050 = 4050,				//RDT作业队列信息查询工作指令清单消息 load list 、discharge list，
        ENUM_QUERY_RDT_CHANGE_VESSEL_4051 = 4051,			//RDT查询转船信息？？
        ENUM_QUERY_RDT_REVESER_DOOR_CTN_4052 = 4052,        //RDT查询调箱门指令信息
        ENUM_QUERY_RDT_BOX_INFO_4053 = 4053,				//RDT 查询集装箱基础信息？？ 信息内容RDT团队提供
        ENUM_QUERY_RDT_ACTIVE_TRUCK_INFO_4054 = 4054,		//RDT查询当前指令可用集卡信息？？
        ENUM_QUERY_RDT_TURN_ON_OR_OFF_4055 = 4055,			//RDT查询当前场地中所有需要插拔电指令信息
        ENUM_QUERY_RDT_SECTION_VIEW_4056 = 4056,			//RDT查询堆场某一贝SectionView信息
        ENUM_QUERY_RDT_BOX_INVENTORY_4057 = 4057,			//RDT查询当前场地中的箱子
        ENUM_QUERY_RDT_VESSEL_SECTION_VIEW_4058 = 4058,		//RDT查询在船舶贝位图信息
        ENUM_QUERY_RDT_APPOINT_CHE_SECTION_VIEW_4059 = 4059, //RDT查询堆场指定贝位的SectionView信息
        ENUM_QUERY_RDT_TRUCKID_SEARCH_DOOR_CTN_4065 = 4065,		//RDT按集卡号查询调箱门指令信息 2013-3-22
        ENUM_QUERY_RDT_BAY_STOWAGE_4067 = 4067,				//室内理货根据指定的船舶贝位，查询当前贝积载信息
        ENUM_QUERY_RDT_SEND_TIME_FOR_SEARCH_4068 = 4068,               //RDT室内理货发送搜索箱量
        ENUM_QUERY_RDT_RH_VESSEL_TK_REFRESH_4069 = 4069,  //手动刷新船图箱量
        ENUM_QUERY_RDT_RH_HCPL_SEARCH_4070 = 4070,        //查询舱盖板销子箱
        ENUM_QUERY_RDT_RH_ERRCTN_SEARCH_4072 = 4072,     //错位箱查询
            ENUM_QUERY_RDT_SEND_TRUCK_MSG_4031 = 4031,            //RDT向集卡终端发送信息

        //from ec sending
        ENUM_ANSWER_SIMULATE_ERR = 0,		                //模拟EC的一个错误应答包(一般为二种，超时或网络中断)

        ENUM_ANSWER_RDT_PARAM_4100 = 4100,					//RDT参数查询结果返回 (应答4000)
        ENUM_ANSWER_RDT_LOGIN_4101 = 4101,					//RDT 用户登录返回 (应答4001)
        ENUM_NTOS_TEXT_MSG_4102 = 4102,						//NTOS给RDT发送文字消息
        ENUM_NTOS_CANCEL_COMMAND_4103 = 4103,				//NTOS撤销指定指令的作业
        ENUM_ANSWER_RDT_TURN_ON_OR_OFF_4104 = 4104,			//NTOS向场地理货RDT发送拔电指令 (应答4055)
        ENUM_NTOS_TO_RDT_MOVE_BOX_4105 = 4105,				//NTOS向场地机械RDT发送移动箱作业指令
        ENUM_NTOS_TO_RDT_GOTO_4106 = 4106,					//NTOS向集卡RDT发送GoTo的目的点
        ENUM_ANSWER_RDT_LIST_4107 = 4107,					//NTOS向场地机械RDT发送可以装箱的集卡清单 (应答4054)
        ENUM_NTOS_TO_RDT_SHIFT_BOX_4108 = 4108,				//NTOS向场地机械RDT发送翻箱指令
        ENUM_ANSWER_RDT_SELECTED_4109 = 4109,				//N-TOS给RDT指令选中消息返回 (应答4003)
        ENUM_ANSWER_RDT_POINT_AND_VESSEL_LOGIN_4110 = 4110,	//N-TOS给RDT理货输入工作点和船舶参考号登录消息返回 (应答4004)
        ENUM_ANSWER_RDT_LOAD_OR_UNLOAD_GOODS_4111 = 4111,	//N-TOS给RDT指令装车、卸车、装船、卸船 确认消息返回 (应答4005)
        ENUM_ANSWER_RDT_STATUS_UPDATE_4112 = 4112,			//RDT状态更新消息返回 (应答4006)
        ENUM_ANSWER_RDT_SHIFT_BOX_4113 = 4113,				//RDT翻箱指令确认消息返回 (应答4007)
        ENUM_ANSWER_RDT_LOGOUT_4114 = 4114,				    //RDT用户退出返回 (应答4008)
        ENUM_ANSWER_RDT_MODIFY_BOX_4116 = 4116,              //RDT修改箱信息消息返回(应答4009)
        ENUM_ANSWER_RDT_CREATEBOX_4117 =4117,               //RDT岸边理货新增溢卸箱报文返回
        ENUM_ANSWER_RDT_ADJUST_SECTION_VIEW_4118 = 4118,	//RDT场地机械终端调整箱位报文返回(应答4018)
        ENUM_NTOS_TO_RDT_MOVE_BOX_4119 = 4119,				//NTOS向场地机械RDT发送移动箱作业指令,优先级为2//4105

        //4152报文ID改成 4120
        //ENUM_ANSWER_RDT_SINGLE_BOX_LOAD_OR_UNLOAD_4152 = 4152,//RDT查询单箱装卸船指令返回 (应答4052)
        ENUM_ANSWER_RDT_SINGLE_BOX_LOAD_OR_UNLOAD_4120 = 4120,//RDT查询单箱装卸船指令返回 (应答4052)

        ENUM_ANSWER_RDT_RESET_CAR_4121 = 4121,				//RDT岸边理货重新确认箱子到集卡返回
        ENUM_ANSWER_RDT_DMS_EXCHANGE_4122 = 4122,			//RDT DMS手动换箱请求返回
        ENUM_QUERY_RDT_RESEVER_CTN_DOOR_4123 = 4123,		//RDT 调箱门指令确认请求返回
        ENUM_ANSWER_RDT_ENTER_HCPL_4124 = 4124,				//RDT录入舱盖销子箱请求返回
        ENUM_ANSWER_RDT_CONTAINER_UPDATE_4125 = 4125,		//室内理货船图箱信息变更返回
        ENUM_ANSWER_RDT_TRUCK_LIST_INFO_4126 = 4126,				//RDT集卡清单信息返回
        ENUM_AMSWER_RDT_SWITCH_BAY_4127 = 4127,				//RDT室内理货当前作业贝切换返回
        ENUM_AMSWER_RDT_RH_CHANGE_WORK_POINT_4128 = 4128,     //RDT室内理货切换工作点返回
        ENUM_AMSWER_RDT_RH_MODIFY_PASSWORD_4129 = 4129,      //RDT室内理货修改密码
        ENUM_ANSWER_RDT_LOGIN_VESSEL_POW_INFO_4130 = 4130,      //RDT登陆室内理货信息返回
        ENUM_ANSWER_RDT_SEND_TRUCK_MSG_4131 = 4131,      //RDT登陆室内理货信息返回
        ENUM_ANSWER_RDT_WORK_LIST_4150 = 4150,				//RDT作业队列信息查询工作指令清单返回？？ (应答4050)
        ENUM_ANSWER_RDT_CHANGE_VESSEL_4151 = 4151,			//RDT查询转船信息返回？？ (应答4051)
        ENUM_QUERY_RDT_REVESER_DOOR_CTN_4152  = 4152,      //RDT查询调箱门信息组返回
        ENUM_ANSWER_RDT_BOX_INFO_4153 = 4153,				//RDT查询集装箱基础信息返回？？信息内容RDT团队提供 (应答4053)
        ENUM_ANSWER_RDT_SECTION_VIEW_4156 = 4156,			//RDT查询堆场某一贝SectionView信息返回(应答4056)
        ENUM_ANSWER_RDT_BOX_INVENTORY_4157 = 4157,			//RDT查询在场箱信息返回(场地核箱)
        ENUM_ANSWER_RDT_VESSEL_SECTION_VIEW_4158 = 4158,	//RDT查询船舶贝位图信息返回(应答4058)
        ENUM_ANSWER_RDT_APPOINT_CHE_SECTION_VIEW_4159 = 4159, //RDT查询指定贝的SectionView信息返回(应答4059)

        ENUM_ANSWER_RDT_SECTION_VIEW_GROUP_4160 = 4160,			//RDT查询堆场某一贝SectionView信息组返回(应答4056)
        ENUM_ANSWER_RDT_VESSEL_SECTION_VIEW_GROUP_4161 = 4161,	    //RDT查询船舶贝位图信息组返回(应答4058)
        ENUM_ANSWER_RDT_APPOINT_CHE_SECTION_VIEW_GROUP_4162 = 4162, //RDT查询指定贝的SectionView信息组返回(应答4059)
        ENUM_QUERY_RDT_TRUCKID_SEARCH_DOOR_CTN_4165 = 4165,	//RDT按集卡号查询调箱门指令信息报文返回 2013-3-22
        ENUM_AMSWER_RDT_BAY_STOWAGE_4167 = 4167,			//室内理货根据指定的船舶贝位，查询当前贝积载信息返回
        ENUM_AMSWER_RDT_SEND_TIME_FOR_SEARCH_4168 = 4168,               //RDT室内理货发送搜索箱量返回
        ENUM_AMSWER_RDT_RH_VESSEL_TK_REFRESH_4169 = 4169,    //手动刷新船图箱量返回
        ENUM_AMSWER_RDT_RH_HCPL_SEARCH_4170 = 4170,         //查询舱盖板销子箱返回
        ENUM_AMSWER_RDT_RH_ERRCTN_SEARCH_4171 = 4171,     //错位箱查询
        ENUM_AMSWER_RDT_RH_ANALYSIS_ERRCTN_4172 = 4172, //解析错位箱大报文

        ENUM_NTOS_TO_RDT_MOVE_BOX_14105 = 14105,				//NTOS向场地机械RDT发送移动箱作业指令
        ENUM_NTOS_TO_RDT_GOTO_14106 = 14106,					//NTOS向集卡RDT发送GoTo的目的点

}ENUM_MSG_TYPE;

#endif // COMMON_H
