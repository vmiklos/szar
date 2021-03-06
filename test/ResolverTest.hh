/// Tests the Resolver interface. Status: tests everything.
class ResolverTest : public TestBase
{
	TEST_SUITE(ResolverTest);
	CPPUNIT_TEST(testMergeSuccess);
	CPPUNIT_TEST(testMergeFailure);
	CPPUNIT_TEST_SUITE_END();

public:
	void testMergeSuccess()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Resolver_var resolver = root->getResolver();
		VersionControl::NamedModel modela;
		modela.name = "foo";
		modela.data = "A\nb\nc\nd\ne\nf\ng\nh\n";
		VersionControl::NamedModel modelb;
		modelb.name = "bar";
		modelb.data = "a\nb\nc\nd\ne\nf\ng\nH\n";
		char* ret = resolver->merge("a\nb\nc\nd\ne\nf\ng\nh\n", modela, modelb);
		CPPUNIT_ASSERT(strcmp(ret, "A\nb\nc\nd\ne\nf\ng\nH\n") == 0);
	}

	void testMergeFailure()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Resolver_var resolver = root->getResolver();
		VersionControl::NamedModel modela;
		modela.name = "foo";
		modela.data = "AA\nb\nc\nd\ne\nf\ng\nh\n";
		VersionControl::NamedModel modelb;
		modelb.name = "bar";
		modelb.data = "BB\nb\nc\nd\ne\nf\ng\nh\n";
		int found = 0;
		try {
			resolver->merge("a\nb\nc\nd\ne\nf\ng\nh\n", modela, modelb);
		} catch (VersionControl::ConflictException& e) {
			found = 1;
			CPPUNIT_ASSERT(strcmp(e.toResolve, "<<<<<<< foo\nAA\n=======\nBB\n>>>>>>> bar\nb\nc\nd\ne\nf\ng\nh\n") == 0);
		}
		CPPUNIT_ASSERT(found == 1);
	}
};
