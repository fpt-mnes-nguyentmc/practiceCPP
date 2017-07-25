#pragma once

#include <vcclr.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <msclr\marshal_cppstd.h>

#include "CLIBasicParam.h"
#include "Gamma3DLogic.h"

using namespace System;

namespace Gamma3DWrapper
{
	public ref class CGamma3DWrapper
	{
	public:
		CGamma3DWrapper();
		~CGamma3DWrapper();

		void ExtractGammaVolume(String^ strEvalFilePath, String^ strRefFilePath);

	private:
		CGamma3DLogic* m_pLogic;
	};
}