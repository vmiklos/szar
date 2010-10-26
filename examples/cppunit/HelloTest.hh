class HelloTest : public CppUnit::TestFixture
{
public:
	void setUp()
	{
	}

	void tearDown() 
	{
	}

	void testFoo()
	{
		CPPUNIT_ASSERT( 1 == 1 );
	}

	static CppUnit::TestSuite *suite()
	{
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "HelloTest" );
		suiteOfTests->addTest( new CppUnit::TestCaller<HelloTest>( 
					"testFoo", 
					&HelloTest::testFoo ) );
		return suiteOfTests;
	}
};
