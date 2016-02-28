#pragma once

namespace SLGE
{
	///vvvvvvvvvvvvvvv///
	//	     WIP	   //
	///vvvvvvvvvvvvvvv///

	struct Local
	{
		int Data;
		int Index;
	};

	struct Global
	{
		std::vector <Local> *Pointer;
		int Index;
	};

	//Used to consolidate the class lists between derived and base classes
	template <class C>
	struct Data_t
	{
		static std::vector <C*> Data;
		static std::vector <Global> Link;
	};

	template <class C>
	std::vector <C*> Data_t <C>::Data;

	template <class C>
	std::vector <Global> Data_t <C>::Link;


	//Used to spawn in all freestanding objects
	//	Usage: 
	//			DynamicClass <ObjectType, ParentClass, ParentClass, ParentClass, etc> ObjectName;
	//
	//	The first class is the class that the object will become, while the following classes are parent classes of the first class
	//	Not all parent classes of the first class must be included
	template <typename C, typename ...D>
	class DynamicClass
	{
		static const int NUMBER_OF_TYPE_ARGS = sizeof...(D) +1;

		//Temp for debug only
		const std::vector <C*> *DATA = &(Data_t <C>::Data);
		const std::vector <Global> *LINK = &(Data_t <C>::Link);


		std::vector <C*> Data;	//Pointers to the actual objects
		std::vector <Local> Link[sizeof...(D) +1]; //Link information to corresponding data in the global array

		template <typename T1>
		void AddPointer()
		{
			const int TYPE_NUM = 0;
			Local TempL;
			Global TempG;

			// Point to the data
			Data_t <T1>::Data.push_back(Data[Data.size() - 1]);

			// Local
			TempL.Data = Data_t <T1>::Data.size() - 1;
			TempL.Index = 0;

			Link[TYPE_NUM].push_back(TempL);

			// Global
			TempG.Pointer = &Link[TYPE_NUM];
			TempG.Index = Link[TYPE_NUM].size() - 1;

			Data_t <T1>::Link.push_back(TempG);

			// Update Local
			Link[TYPE_NUM][Link[TYPE_NUM].size() - 1].Index = Data_t <T1>::Link.size() - 1;
		}

		template <typename T1, typename T2, typename ...Args>
		void AddPointer()
		{
			const int TYPE_NUM = sizeof...(Args) +1;
			Local TempL;
			Global TempG;

			// Point to the data
			Data_t <T1>::Data.push_back(Data[Data.size() - 1]);

			// Local
			TempL.Data = Data_t <T1>::Data.size() - 1;
			TempL.Index = 0;

			Link[TYPE_NUM].push_back(TempL);

			// Global
			TempG.Pointer = &Link[TYPE_NUM];
			TempG.Index = Link[TYPE_NUM].size() - 1;

			Data_t <T1>::Link.push_back(TempG);

			// Update Local
			Link[TYPE_NUM][Link[TYPE_NUM].size() - 1].Index = Data_t <T1>::Link.size() - 1;

			AddPointer <T2, Args...>();
		}

		template <typename T1>
		void DeletePointer(int in_Index)
		{
			const int TYPE_NUM = 0;

			//std::vector	 Data_t <T1>::Link[Link[TYPE_NUM][i].Index].Index

			//Shift all local indexes behind the deleted data back one
			for (int i = 0; i < (int) Data_t <T1>::Link.size(); i++)
			{
				// (*PointerToVec)[IndexOfVec].data >= IndexOfDataDeleted
				if ((*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Data > Link[TYPE_NUM][in_Index].Data)
					(*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Data--;

				// (*PointerToVec)[IndexOfVec].index >= IndexOfIndexDeleted
				if ((*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Index > Link[TYPE_NUM][in_Index].Index)
					(*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Index--;
			}

			//Shift the global indexes behind the deleted data back one
			for (int i = 0; i < (int) Link[TYPE_NUM].size(); i++)
			{
				// Skip deleted index
				if (i == in_Index)
					continue;

				// IndexToObjectIndex[IndexOfGlobal + 1] > IndexDeleted
				if (Data_t <T1>::Link[Link[TYPE_NUM][i].Index + 1].Index > in_Index)
					Data_t <T1>::Link[Link[TYPE_NUM][i].Index + 1].Index--;
			}

			//Delete Data						  
			Data_t <T1>::Data.erase(Data_t <T1>::Data.begin() + Link[TYPE_NUM][in_Index].Data);
			Data_t <T1>::Link.erase(Data_t <T1>::Link.begin() + Link[TYPE_NUM][in_Index].Index);
			Link[TYPE_NUM].erase(Link[TYPE_NUM].begin() + in_Index);
		}

		template <typename T1, typename T2, typename ...Args>
		void DeletePointer(int in_Index)
		{
			const int TYPE_NUM = sizeof...(Args) +1;

			//std::vector	 Data_t <T1>::Link[Link[TYPE_NUM][i].Index].Index

			//Shift all local indexes behind the deleted data back one
			for (int i = 0; i < (int) Data_t <T1>::Link.size(); i++)
			{
				// (*PointerToVec)[IndexOfVec].data >= IndexOfDataDeleted
				if ((*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Data > Link[TYPE_NUM][in_Index].Data)
					(*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Data--;

				// (*PointerToVec)[IndexOfVec].index >= IndexOfIndexDeleted
				if ((*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Index > Link[TYPE_NUM][in_Index].Index)
					(*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Index--;
			}

			//Shift the global indexes behind the deleted data back one
			for (int i = 0; i < (int) Link[TYPE_NUM].size(); i++)
			{
				// Skip deleted index
				if (i == in_Index)
					continue;

				// IndexToObjectIndex[IndexOfGlobal + 1] > IndexDeleted
				if (Data_t <T1>::Link[Link[TYPE_NUM][i].Index + 1].Index > in_Index)
					Data_t <T1>::Link[Link[TYPE_NUM][i].Index + 1].Index--;
			}

			//Delete Data						  
			Data_t <T1>::Data.erase(Data_t <T1>::Data.begin() + Link[TYPE_NUM][in_Index].Data);
			Data_t <T1>::Link.erase(Data_t <T1>::Link.begin() + Link[TYPE_NUM][in_Index].Index);
			Link[TYPE_NUM].erase(Link[TYPE_NUM].begin() + in_Index);

			DeletePointer <T2, Args...>(in_Index);
		}

		template <typename Type, typename Comparer>
		int GetTypeIndex(int Count)
		{
			if (std::is_same <Type, Comparer>::value)
				return Count;
			else
				return 0;
		}

		template <typename Type, typename Comparer, typename Comparer2, typename ...Args>
		int GetTypeIndex(int Count)
		{
			if (std::is_same <Type, Comparer>::value)
				return Count;
			else
				return GetTypeIndex<Type, Comparer2, Args...>(Count - 1);
		}


	public:

	//Local
		//Clears the internal variables
		DynamicClass()
		{
			Data.empty();

			for (int i = 0; i < NUMBER_OF_TYPE_ARGS; i++)
				Link[i].empty();
		}

		//Spawns in the objects
		//	@Amount is the number of objects to spawn in
		//	@WindowHandle is the window to spawn them onto
		//		If NULL the class is not registered with a window. Useful for classes that are not displayed
		void Spawn(int Amount, Window_t *WindowHandle = nullptr)
		{
			for (int i = 0; i < Amount; i++)
			{
				if (WindowHandle == nullptr)
					Data.push_back(new C());
				else
					Data.push_back(new C(WindowHandle));

				//Loop through the remaining class types
				AddPointer <C, D...>();
			}
		}

		//Despawns all objects
		void Despawn()
		{
			Despawn(0, Data.size());
		}

		//Despawns a specified number of objects
		//	@in_Index is the index of the first object to despawn
		//	@in_Amount is the total number of objects to despawn
		void Despawn(int in_Index, int in_Amount)
		{
			if (in_Amount > 0 && Data.size() > 0)
			{
				for (int i = in_Index; i < in_Index + in_Amount; i++)
				{
					DeletePointer <C, D...>(in_Index);

					delete Data[in_Index];
					Data.erase(Data.begin() + in_Index);
				}
			}
		}

		//Returns the number of objects spawned
		int NumberOfObjects()
		{
			return Data.size();
		}

		//Gets the position of the object in the global array
		template <typename T1 = C>
		int GetPosition(int in_Index = 0)
		{
			if (in_Index >= 0 && in_Index < Link[0].size())
			{
				int Type = GetTypeIndex <T1, C, D...>(NUMBER_OF_TYPE_ARGS - 1);

				return Link[Type][in_Index].Data;
			}
			else
				return -1;
		}

		//Returns a modifiable object at the specified index
		//	@in_Index is the index of the spawned object
		C &operator[](unsigned in_Index)
		{
			if (in_Index >= 0 && in_Index < Data.size())
				return *(Data[in_Index]);

			return *(Data[0]);
		}

		//Returns a non modifiable object at the specified index
		//	@in_Index is the index of the spawned object
		const C &operator[](unsigned in_Index) const
		{
			if (in_Index >= 0 && in_Index < Data.size())
				return *(Data[in_Index]);

			return *(Data[0]);
		}
	};

	
//Global
	//Returns a non modifiable object at the specified index
	//	@in_Index is the index of the spawned object
	template <typename C>
	C &Get(unsigned in_Index)
	{
		if (in_Index >= 0 && in_Index < Data_t <C>::Data.size())
			return *(Data_t <C>::Data[in_Index]);

		return *(Data_t <C>::Data[0]);
	}

	template <typename C>
	C &GetByID(const char* ID)
	{
		for (int i = 0; i < Data_t <C>::Data.size(); i++)
		{
			if (!strcmp(Data_t <C>::Data[i]->GetID(), ID))
				return *(Data_t <C>::Data[i]);
		}

		return nullptr;// *(Data_t <C>::Data[0]);
	}

   	//Swaps the position of two objects in the global array
	//	@Position is the location of the first object
	//	@NewPosition is the location of the second object
	template <typename C>
	void Swap(int Position, int NewPosition)
	{
		if (Position >= 0 && NewPosition >= 0 && Position < Data_t <C>::Data.size() && NewPosition < Data_t <C>::Data.size())
		{
			Global Temp2 = Data_t <C>::Link[Position];
			C* Temp3 = Data_t <C>::Data[Position];

			Data_t <C>::Link[Position] = Data_t <C>::Link[NewPosition];
			Data_t <C>::Link[NewPosition] = Temp2;

			Data_t <C>::Data[Position] = Data_t <C>::Data[NewPosition];
			Data_t <C>::Data[NewPosition] = Temp3;
		}
	}

	//Moves an object to a new position in the global array
	//	@Position is the position of the object to move
	//	@NewPosition is the position to move the object to
	template <typename C>
	void Move(int Position, int NewPosition)
	{
		if (Position >= 0 && NewPosition >= 0 && Position < Data_t <C>::Data.size() && NewPosition < Data_t <C>::Data.size())
		{
			if (Position > NewPosition)
			{
				for (int i = NewPosition; i < Position - 1; i++)
					Swap <C> (i, i + 1);
			}

			else if (Position < NewPosition)
			{
				for (int i = Position; i < NewPosition - 1; i++)
					Swap <C> (i, i + 1);
			}
		}
	}

	//Executes a method for all objects in the global array
	//	@Function is a pointer to a method to be called on all objects
	//	@...Args is a variadic argument for the arguments in the method
	//	
	//	Usage:
	//			DynamicClass <Class>::All(&Class::Method(), Arg1, Arg2, etc);
	template <class C, typename F, typename ...A>
	void All(F Function, A... Args)
	{
		for (int i = 0; i < (int) Data_t <C>::Data.size(); i++)
			(Data_t <C>::Data[i]->*Function)(Args...);
	}

	//Retuns the number of objects in the global array	
	template <typename C>
	int NumberOfObjects()
	{
		return Data_t <C>::Data.size();
	}

	///^^^^^^^^^^^^^^^///
	//	     WIP	   //
	///^^^^^^^^^^^^^^^///
}


			