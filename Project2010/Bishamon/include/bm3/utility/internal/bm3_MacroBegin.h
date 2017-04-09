#define	BM3_DECLARE_SETTER_GETTER_REF(TYPE, NAME)							\
	public:																	\
		typedef	TYPE	NAME ## InternalType;								\
																			\
		const TYPE	&NAME() const{					return NAME ## _;	}	\
		TYPE		&NAME(){						return NAME ## _;	}	\
		void		Set ## NAME(const TYPE &v){		NAME ## _ = v;		}	\
/*	private:	*/															\
		TYPE		NAME ## _

#define	BM3_DECLARE_SETTER_GETTER_VALUE(TYPE, NAME)							\
	public:																	\
		typedef	TYPE	NAME ## InternalType;								\
																			\
		TYPE		NAME() const{					return NAME ## _;	}	\
		void		Set ## NAME(TYPE v){			NAME ## _ = v;		}	\
/*	private:	*/															\
		TYPE		NAME ## _

#define	BM3_DECLARE_SETTER_REF(TYPE, NAME)									\
	public:																	\
		typedef	TYPE	NAME ## InternalType;								\
																			\
		void		Set ## NAME(const TYPE &v){		NAME ## _ = v;		}	\
/*	private:	*/															\
		TYPE		NAME ## _

#define	BM3_DECLARE_SETTER_VALUE(TYPE, NAME)								\
	public:																	\
		typedef	TYPE	NAME ## InternalType;								\
																			\
		void		Set ## NAME(TYPE v){			NAME ## _ = v;		}	\
/*	private:	*/															\
		TYPE		NAME ## _

#define	BM3_DECLARE_GETTER_REF(TYPE, NAME)									\
	public:																	\
		typedef	TYPE	NAME ## InternalType;								\
																			\
		const TYPE	&NAME() const{					return NAME ## _;	}	\
		TYPE		&NAME(){						return NAME ## _;	}	\
/*	private:	*/															\
		TYPE		NAME ## _

#define	BM3_DECLARE_GETTER_VALUE(TYPE, NAME)								\
	public:																	\
		typedef	TYPE	NAME ## InternalType;								\
																			\
		TYPE		NAME() const{					return NAME ## _;	}	\
/*	private:	*/															\
		TYPE		NAME ## _
