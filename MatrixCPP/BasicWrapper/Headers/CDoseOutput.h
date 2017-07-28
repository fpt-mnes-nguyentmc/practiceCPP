struct CDoseOutput
{
public:
	/// <summary>
	/// The pDoseEval
	/// </summary>
	float **pDoseEval;

	/// <summary>
	/// The pDoseRef
	/// </summary>
	float **pDoseRef;

	/// <summary>
	/// The X, Y, Z size 
	/// </summary>
	int m_iWsize;

	CDoseOutput()
	{
		this->pDoseEval = NULL;
		this->pDoseRef = NULL;
		this->m_iWsize = 0;
	}
};