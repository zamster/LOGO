#ifndef SINGLETON_H_
#define SINGLETON_H_

#define INITIALISESINGLETON(type) \
  template <> type * Singleton <type> :: mSingleton = 0
  
template< class T > class Singleton
{
protected:
	static T* mSingleton;
	Singleton()
	{
		mSingleton = static_cast<T*>(this);
	}
	
public:

	virtual ~Singleton()
	{
		mSingleton = 0;
	}
	static T& getSingleton()
	{
		return *mSingleton;
	}
	static T* getSingletonPtr()
	{ 
		return mSingleton;
	}

};
#endif
