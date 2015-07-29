#pragma once

template <class T>
void SerializeT(char* Buffer, T Data, int Type)
{
	Buffer[0] = Type;
	union {
		T				Data;
		unsigned char	bytes[sizeof(T)];
	} u;
	u.Data = Data;
	for (int i = 0; i < sizeof(T); ++i)
		Buffer[i + 1] = u.bytes[i];
}

template <class T>
void DeserializeT(char* Buffer, T *Data, int *Type)
{
	union {
		T				Data;
		unsigned char	bytes[sizeof(T)];
	} u;
	for (int i = 0; i < sizeof(T); i++)
		u.bytes[i] = Buffer[i + 1];
	*Data = u.Data;
	if (Type)
		*Type = Buffer[0];
	if (*Type < 0) *Type += 256;
}

template <class T>
void DeserializeT(char* Buffer, T *Data)
{
	union {
		T				Data;
		unsigned char	bytes[sizeof(T)];
	} u;
	for (int i = 0; i < sizeof(T); i++)
		u.bytes[i] = Buffer[i + 1];
	*Data = u.Data;
}

