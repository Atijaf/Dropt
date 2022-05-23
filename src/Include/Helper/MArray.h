#pragma once
#include <cstdint>

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
			MArray(uint32_t InitialSize) :
				Size(InitialSize),
				ArrayOfElements(new T[Size]) {};
			~MArray();

			bool Resize(uint32_t NewSize);
			bool DynamicIncrementSize();
			bool AddElement(T Element);

			uint32_t GetSize() const { return Size; }
			uint32_t GetNumOfElements() const { return NumOfElements; }

			void Sort();

			// Returns Element at Index of Array.  Unsafe
			T& operator[](std::size_t Index) {
				return ArrayOfElements[Index];
			}
			const T& operator[](std::size_t Index) const {
				return ArrayOfElements[Index];
			}
		protected:

			bool NeedsResize() const { return NumOfElements == Size; }

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
			if (NewSize = Size) return false;

			T* NewContainer = new T[NewSize];
			for (uint32_t i = 0; i < ((NewSize > NumOfElements) ? NumOfElements : NewSize); ++i)
				NewContainer[i] = ArrayOfElements[i];
			delete[] ArrayOfElements;
			ArrayOfElements = NewContainer;
			return false;
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

			auto Compare = [](const void* A, const void* B) {
				const T* APtr = (const T*)(A);
				const T* BPtr = (const T*)(B);
				if (APtr > BPtr)
					return -1;
				if (APtr < BPtr)
					return 1;
				return 0;
			};
			std::qsort(this->ArrayOfElements, NumOfElements, sizeof(ArrayOfElements[0]), Compare);
		}
	}
}