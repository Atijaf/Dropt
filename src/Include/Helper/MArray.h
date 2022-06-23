#pragma once
#include <cstdint>
#include <functional>
#include <algorithm>

namespace Dropt {
	namespace Helper
	{

		/// <summary>
		/// Basic Array Wrapper that contains an array of pointers to type T
		/// - Does not delete the pointer
		/// </summary>
		template<typename T>
		class MArray {
		public:

			/// <summary>
			/// Initializes array
			/// </summary>
			/// <param name="InitialSize">:	Starting size of array</param>
			/// 
			/// <remarks>
			/// Default sorting of array is least to greatest
			/// </remarks>
			MArray(uint32_t InitialSize) :
				Size(InitialSize),
				ArrayOfElements(new T[Size]) {};

			MArray(const MArray<T>& OG) :
				Size(OG.GetSize()),
				ArrayOfElements(new T[Size]) 
			{
				for (int i = 0; i < Size; ++i)
					ArrayOfElements[i] = OG.ArrayOfElements[i];
			}
			~MArray();

			bool Resize(uint32_t NewSize);
			void ShrinkToFit();
			bool DynamicIncrementSize();
			bool AddElement(T Element);

			uint32_t GetSize() const { return Size; }
			uint32_t GetNumOfElements() const { return NumOfElements; }

			/// <summary>
			/// Sorts array using Compare Function.
			/// </summary>
			/// <param name="Func"> Must return bool and accept two parameters of type Stored in array</param>
			void Sort();
			void Sort(std::function<bool(const T A, const T B)> SortFunc);

			// Returns Element at Index of Array.  Unsafe
			T& operator[](std::size_t Index) {
				return ArrayOfElements[Index];
			}
			const T& operator[](std::size_t Index) const {
				return ArrayOfElements[Index];
			}
		protected:

			bool NeedsResize() const { return NumOfElements == Size; }

			// Function to compare elements to one another.  Used for sorting array
			std::function<bool(const T A, const T B)> Func_Sort = [](T A, T B) {return (A < B); };

			uint32_t Size = 1;
			uint32_t NumOfElements = 0;
			T* ArrayOfElements = nullptr;
		};


		template<typename T>
		inline MArray<T>::~MArray()
		{
			if (Size > 0)
				delete[] ArrayOfElements;
		}

		template<typename T>
		inline bool MArray<T>::Resize(uint32_t NewSize)
		{
			if (NewSize == Size) return false;
			if (NewSize > 0) {
				T* NewContainer = new T[NewSize];
				for (uint32_t i = 0; i < ((NewSize > NumOfElements) ? NumOfElements : NewSize); ++i)
					NewContainer[i] = ArrayOfElements[i];
				delete[] ArrayOfElements;
				ArrayOfElements = NewContainer;
			}
			else {
				delete[] ArrayOfElements;
			}
			NumOfElements = NewSize;
			Size = NewSize;
			return true;
		}

		template<typename T>
		inline void MArray<T>::ShrinkToFit()
		{
			if (Size == NumOfElements)
				return;
			Resize(NumOfElements);
		}

		template<typename T>
		inline bool MArray<T>::DynamicIncrementSize()
		{
			const uint8_t MaxIncrement = 100;
			uint8_t IncrementAmount = 10;
			if (Size / 2 > MaxIncrement)
				IncrementAmount = MaxIncrement;

			// Protect against overflow
			if (Size + IncrementAmount < Size) {
				// If Size is already maxed out, array can't be resized
				if (Size == 4294967295)
					return false;
				return Resize(4294967295);
			}
			return Resize(Size + IncrementAmount);
		}

		template<typename T>
		inline bool MArray<T>::AddElement(T Element)
		{
			// Resize if needed.  Return false if resize fails
			if (NeedsResize() && !DynamicIncrementSize())
				return false;

			ArrayOfElements[NumOfElements++] = Element;
			return true;
		}


		

		template<typename T>
		inline void Dropt::Helper::MArray<T>::Sort()
		{
			if (NumOfElements == 0)
				return;

			auto SortFunc = [](T& A, T& B)
			{
				return A < B;
			};

			std::sort(ArrayOfElements, ArrayOfElements + NumOfElements, SortFunc);
		}

		template<typename T>
		inline void Dropt::Helper::MArray<T>::Sort(std::function<bool(const T A, const T B)> SortFunc)
		{
			if (NumOfElements == 0)
				return;

			std::sort(ArrayOfElements, ArrayOfElements + NumOfElements, SortFunc);
		}
	}
}